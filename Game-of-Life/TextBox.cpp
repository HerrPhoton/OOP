#include "TextBox.h"

TextBox::TextBox() {

	shape.setFillColor(sf::Color::White);
	shape.setOutlineColor(sf::Color{ 150, 150, 150, 255 });
	shape.setOutlineThickness(4);

	font.loadFromFile("segoepr.ttf");
	text.setFont(font);
	text.setCharacterSize(50);
	text.setString("B3 S23 F40");
}

bool TextBox::check_click(sf::Vector2i pos) {

	sf::Vector2f null_point = shape.getPosition() - shape.getOrigin();

	if (pos.x >= null_point.x && pos.x <= null_point.x + shape.getSize().x)
		if (pos.y >= null_point.y && pos.y <= null_point.y + shape.getSize().y)
		{
			change_color(true);
			active = true;
			return true;
		}

	change_color(false);
	active = false;
	return false;
}

void TextBox::set_size(sf::Vector2f size) {

	shape.setSize(size);
	shape.setOrigin(shape.getSize().x / 2, 0);
}

void TextBox::set_pos(sf::Vector2f pos) {

	sf::Vector2f null_point = pos - shape.getOrigin();

	shape.setPosition(pos);
	text.setPosition(null_point.x + 20, pos.y + shape.getSize().y / 5.f);
}

void TextBox::set_text(std::string str) {

	text.setString(str);
	text.setOrigin(0, text.getGlobalBounds().getSize().y / 2.f);
}

void TextBox::set_active_color(sf::Color color) {

	active_color = color;
}

void TextBox::set_inactive_color(sf::Color color) {

	text.setFillColor(color);
	inactive_color = color;
}

void TextBox::change_color(bool is_active) {

	if (is_active)
		text.setFillColor(active_color);
	else
		text.setFillColor(inactive_color);

}

void TextBox::add_char(char chr) {

	if (!active)
		return;

	std::string tmp = text.getString();

	if (tmp.size() == 26)
		return;

	tmp.push_back(chr);
	text.setString(tmp);

	sf::Vector2f null_point = shape.getPosition() - shape.getOrigin();
	if (text.getPosition().x + text.getLocalBounds().getSize().x > null_point.x + shape.getSize().x - shape.getOutlineThickness() || shrinked_cnt)
	{
		text.setCharacterSize(text.getCharacterSize() - text.getCharacterSize() / 10);
		shrinked_cnt++;
	}
}

 void TextBox::delete_char() {

	if (!active)
		return;

	std::string tmp = text.getString();

	if (!tmp.size())
		return;

	tmp.pop_back();
	
	if (shrinked_cnt)
	{
		text.setCharacterSize(text.getCharacterSize() + text.getCharacterSize() / 10);
		shrinked_cnt--;
	}
	
	text.setString(tmp);

}

void TextBox::clear_text() {

	if (!active)
		return;

	text.setString("");
	text.setCharacterSize(50);
	shrinked_cnt = 0;
}

std::string TextBox::get_text() {

	return text.getString();
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(shape, states);
	target.draw(text, states);
}
