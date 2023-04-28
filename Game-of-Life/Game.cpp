#include "Game.h"
#include <chrono>
#include <thread>
#include <cmath>

Game::Game(int win_width, int win_height, int offset) {

	field_size = win_height - 2 * offset;
	field_offset = offset;
	cell_size = float(field_size) / cells_in_row;

	act_text.second = "Actively";
	nAct_text.second = "Inactive";
	
	float middle_x = win_height + (win_width - win_height) / 2;
	text.setPosition(middle_x, offset);
	
	text.setString(act_text.second);
	act_text.first.x = std::round((text.getGlobalBounds().getSize() / 2.f).x);
	act_text.first.y = 0;

	text.setString(nAct_text.second);
	nAct_text.first.x = std::round((text.getGlobalBounds().getSize() / 2.f).x);
	nAct_text.first.y = 0;

	text.setOrigin(nAct_text.first);
	change_text(false);

	sf::Vector2f button_size(300, 100);
	int button_offset = 30;
	sf::Color gray = sf::Color(150, 150, 150, 255);

	clear.set_size(button_size);
	clear.set_pos(sf::Vector2f(middle_x, 2 * (button_size.y + button_offset) + win_height / 2.5));
	clear.set_nClick_color(sf::Color::White);
	clear.set_clicked_color(sf::Color::Red);
	clear.set_disabled_color(gray);
	clear.set_text("Clear");

	start.set_size(button_size);
	start.set_pos(sf::Vector2f(middle_x, button_size.y + button_offset + win_height / 2.5));
	start.set_nClick_color(sf::Color::White);
	start.set_clicked_color(sf::Color::Red);
	start.set_disabled_color(gray);
	start.set_text("Start");

	accept.set_size(button_size);
	accept.set_pos(sf::Vector2f(middle_x, win_height / 2.5));
	accept.set_nClick_color(sf::Color::White);
	accept.set_clicked_color(sf::Color::Red);
	accept.set_disabled_color(gray); 
	accept.set_text("Accept");

	options.set_size(sf::Vector2f(450, 100));
	options.set_pos(sf::Vector2f(middle_x, win_height / 5));
	options.set_active_color(sf::Color::Black);
	options.set_inactive_color(gray);

	std_rools(true);
	std_rools(false);

	init_cells();
}

void Game::mouse_clicked(sf::Vector2i pos) {

	sf::Vector2i ids = get_Cell_byPos(pos);
	
	if (options.check_click(pos))
		return;

	else if (ids.x != -1)
		cell_clicked(ids);

	else if (clear.check_click(pos))
		kill_all();

	else if (start.check_click(pos))
	{
		start.set_enabled(false);
		game_step();
	}

	else if (accept.check_click(pos))
		change_options(options.get_text());
}

void Game::cell_clicked(sf::Vector2i pos){

	if (work)
		return;

	if (pos.x == -1)
		return;

	int id = cells_in_row * pos.x + pos.y;

	if (!pressed)
	{
		if (cells[id].is_live)
			pressed_painted = true;

		pressed = true;
	}

	if (cells[id].is_live != pressed_painted)
		return;

	change_cellLife(id, false);
}

void Game::reset() {

	pressed_painted = false;
	pressed = false;
	work = false;
	change_text(false);
	start.set_enabled(true);
}

void Game::kill_all() {

	for (auto& cell : cells)
		cell.is_live = false;

	reset();
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	UI::draw(target, states, cells);
}

void Game::init_cells() {

	cells.resize(cells_in_row * cells_in_row);

	for (size_t i = 0; i < cells_in_row; i++)
		for (size_t j = 0; j < cells_in_row; j++)
		{
			int cnt = 0;
			int id = cells_in_row * i + j;

			for (int n = -1; n < 2; n++)
				for (int m = -1; m < 2; m++)
				{
					if (!n && !m)
						continue;

					int row = i + n;
					int col = j + m;

					if (row < 0)
						row = cells_in_row - 1;
					else if (row > cells_in_row - 1)
						row = 0;

					if (col < 0)
						col = cells_in_row - 1;
					else if (col > cells_in_row - 1)
						col = 0;

					cells[id].neighbors[cnt++] = &cells[cells_in_row * row + col];
				}
		}
}

bool Game::check_birth_cnt(int cnt) {

	for (auto& num : birth_cnt)
		if (num == cnt)
			return true;
	
	return false;

}

bool Game::check_survive_cnt(int cnt) {

	for (auto& num : survive_cnt)
		if (num == cnt)
			return true;

	return false;

}

void Game::change_cellLife(int id, bool is_next) {

	if (!is_next)
		cells[id].is_live = !cells[id].is_live;
	else
		cells[id].next_live = !cells[id].is_live;
}

bool Game::apply_live() {
	
	bool found_live = false;

	for (auto& cell : cells)
	{
		cell.is_live = cell.next_live;

		if (cell.is_live)
			found_live = true;
	}

	return found_live;
}

bool Game::is_same_states() {

	for (auto& cell : cells)
		if (cell.is_live != cell.next_live)
			return false;

	return true;
}

void Game::change_options(std::string text) {

	std::transform(text.begin(), text.end(), text.begin(), tolower);

	std::string temp;

	while (true) {

		temp = text.substr(0, text.find(' '));

		if (temp.size())
		{
			switch (temp[0])
			{
			case 'b':

				birth_cnt.clear();

				for (auto chr : temp)
					if (isdigit(chr) && chr - '0' < 9)
						birth_cnt.insert(chr - '0');

				if (!birth_cnt.size())
					std_rools(true);

				break;

			case 's':

				survive_cnt.clear();

				for (auto chr : temp)
					if (isdigit(chr) && chr - '0' < 9)
						survive_cnt.insert(chr - '0');

				if (!survive_cnt.size())
					std_rools(false);

				break;

			case 'f':

				std::string temp1 = temp.substr(1);
				int cnt = 40;

				if (temp1.size())
				{
					try
					{
						cnt = std::stoi(temp1);

						if (cnt < 2 || cnt > 100)
							break;
					}

					catch (...)
					{
						break;
					}
				}

				reset();
				cells_in_row = cnt;
				cell_size = float(field_size) / cells_in_row;
				cells.clear();
				init_cells();

				break;
			}

			if (temp.size() == text.size())
				break;

			else
				text = text.substr(temp.size() + 1);
		}

		else
			break;
	}
}

void Game::std_rools(bool for_birth) {

	if (for_birth)
		birth_cnt.insert(3);

	else
	{
		survive_cnt.insert(2);
		survive_cnt.insert(3);
	}
}

void Game::game_step() {

	if (!work)
	{
		work = true;
		change_text(true);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(delay));

	for (size_t i = 0; i < cells_in_row; i++)
		for (size_t j = 0; j < cells_in_row; j++)
		{
			int id = cells_in_row * i + j;
			int neighbors = cells[id].get_neighbors();

			if (!cells[id].is_live)
			{
				if (check_birth_cnt(neighbors))
					change_cellLife(id, true);

				else
					cells[id].next_live = cells[id].is_live;
			}
			
			else
			{
				if (!check_survive_cnt(neighbors))
					change_cellLife(id, true);

				else
					cells[id].next_live = cells[id].is_live;
			}
		}
	
	if (is_same_states())
		work = false;

	if (work && !apply_live())
		work = false;

	if (!work)
	{
		change_text(false);
		start.set_enabled(true);
	}
}

bool Game::is_work() {
	return work;
}

