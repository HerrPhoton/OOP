#include "Button.h"

Button::Button() {

	font.loadFromFile("segoepr.ttf");
	text.setFont(font);
	text.setCharacterSize(50);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineThickness(3);
}

bool Button::check_click(sf::Vector2i pos) {

	if (!enabled)
		return false;

	sf::Vector2f null_point = shape.getPosition() - shape.getOrigin();

	if (pos.x >= null_point.x && pos.x <= null_point.x + shape.getSize().x)
		if (pos.y >= null_point.y && pos.y <= null_point.y + shape.getSize().y)
		{
			change_color(true);
			return true;
		}

	return false;
}

void Button::change_color(bool is_clicked) {

	if (!enabled)
	{
		shape.setOutlineColor(disabled);
		text.setFillColor(disabled);
		return;
	}

	if (is_clicked)
	{
		text.setFillColor(clicked);
		shape.setOutlineColor(clicked);
	}

	else
	{
		text.setFillColor(not_clicked);
		shape.setOutlineColor(not_clicked);
	}

}

void Button::set_enabled(bool is_enabled) {

	if (is_enabled)
	{
		enabled = true;
		shape.setOutlineColor(not_clicked);
	}

	else
		enabled = false;
}

void Button::set_size(sf::Vector2f size) {

	shape.setSize(size);
	shape.setOrigin(shape.getSize().x / 2, 0);
}

void Button::set_pos(sf::Vector2f pos) {

	shape.setPosition(pos);
	text.setPosition(pos.x, pos.y + shape.getSize().y / 3.f);
}

void Button::set_nClick_color(sf::Color color) {

	text.setFillColor(color);
	not_clicked = color;
}

void Button::set_clicked_color(sf::Color color) {

	clicked = color;
}

void Button::set_disabled_color(sf::Color color) {

	disabled = color;
}

void Button::set_text(std::string str) {

	text.setString(str);
	text.setOrigin(text.getGlobalBounds().getSize() / 2.f);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(shape, states);
	target.draw(text, states);
}