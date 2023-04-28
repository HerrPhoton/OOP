#pragma once

#include <SFML/Graphics.hpp>
#include "Cell.h"
#include "Button.h"
#include "TextBox.h"

class UI : public sf::Drawable, public sf::Transformable {

public:
	UI();
	~UI() = default;

public:
	sf::Vector2i get_Cell_byPos(sf::Vector2i);
	void reset_buttons();
	void get_event(sf::Event);

public:
	void draw(sf::RenderTarget& target, sf::RenderStates states, std::vector<Cell>) const;

protected:
	void change_text(bool);

protected:
	int cells_in_row = 40;
	int field_offset = 0;
	int field_size = 0;
	float cell_size = 0;

protected:
	sf::Font font;
	sf::Text text;

protected:
	std::pair <sf::Vector2f, std::string> act_text;
	std::pair <sf::Vector2f, std::string> nAct_text;

protected:
	Button clear;
	Button start;
	Button accept;

protected:
	TextBox options;
};