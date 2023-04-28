#pragma once

#include <vector>
#include "UI.h"
#include <set>

class Game: public UI, public Cell {

public:
	Game() = default;
	Game(int win_width, int win_height, int offset);
	~Game() = default;

public:
	void mouse_clicked(sf::Vector2i);
	void cell_clicked(sf::Vector2i);
	void reset();
	void kill_all();
	void game_step();
	bool is_work();

public:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void init_cells();
	bool check_birth_cnt(int);
	bool check_survive_cnt(int);
	void change_cellLife(int, bool is_next = false);
	bool apply_live();
	bool is_same_states();
	void change_options(std::string);
	void std_rools(bool);

protected:
	std::vector<Cell> cells;
	std::vector<Cell> changed_cells;

protected:
	std::set<int> birth_cnt;
	std::set<int> survive_cnt;

protected:
	bool pressed_painted = false;
	bool pressed = false;
	
protected:
	bool work = false;
	int delay = 40;

};

