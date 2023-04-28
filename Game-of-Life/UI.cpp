#include "UI.h"

UI::UI() {

	font.loadFromFile("segoepr.ttf");
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(70);
}

sf::Vector2i UI::get_Cell_byPos(sf::Vector2i pos) {

	int coll_cell = (pos.x - field_offset) / cell_size;
	int row_cell = (pos.y - field_offset) / cell_size;

	if (coll_cell < 0 || coll_cell > cells_in_row - 1)
		return sf::Vector2i(-1, -1);

	if (row_cell < 0 || row_cell > cells_in_row - 1)
		return sf::Vector2i(-1, -1);

	return sf::Vector2i(row_cell, coll_cell);
}

void UI::draw(sf::RenderTarget& target, sf::RenderStates states, std::vector<Cell> cells) const {

	states.transform *= getTransform();

	sf::RectangleShape shape(sf::Vector2f(cell_size, cell_size));
	shape.setOutlineThickness(1);
	shape.setOutlineColor(sf::Color(sf::Color::Cyan));

	target.draw(text, states);

	for (size_t i = 0; i < cells_in_row; i++)
		for (size_t j = 0; j < cells_in_row; j++)
		{
			if (!cells[cells_in_row * i + j].is_live)
				shape.setFillColor(sf::Color::Transparent);
			else
				shape.setFillColor(sf::Color::Blue);

			shape.setPosition(j * cell_size, i * cell_size);
			target.draw(shape, states);
		}

	target.draw(clear, states);
	target.draw(start, states);
	target.draw(accept, states);
	target.draw(options, states);
}

void UI::change_text(bool is_act_text) {

	if (is_act_text)
	{
		text.setOrigin(act_text.first);
		text.setString(act_text.second);
	}
	
	else
	{
		text.setOrigin(nAct_text.first);
		text.setString(nAct_text.second);
	}
}

void UI::reset_buttons() {

	clear.change_color(false);
	start.change_color(false);
	accept.change_color(false);
}

void UI::get_event(sf::Event event) {

	switch (event.type)
	{

	case sf::Event::TextEntered:

		if (event.text.unicode < 0 || event.text.unicode > 255)
			break;

		if (isdigit(event.text.unicode) || isalpha(event.text.unicode))
			options.add_char(event.text.unicode);

		break;
	
	case sf::Event::KeyPressed:

		switch (event.key.code)
		{
		case sf::Keyboard::BackSpace:
			options.delete_char();
			break;

		case sf::Keyboard::Space:
			options.add_char(' ');
			break;

		case sf::Keyboard::Delete:
			options.clear_text();
			break;
		}

		break;
	}


}

