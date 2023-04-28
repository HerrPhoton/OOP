#pragma once

#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {

public:
    Button();
    ~Button() = default;

public:
    bool check_click(sf::Vector2i);
    void change_color(bool);
    void set_enabled(bool);

public:
    void set_size(sf::Vector2f);
    void set_pos(sf::Vector2f);
    void set_nClick_color (sf::Color);
    void set_clicked_color(sf::Color);
    void set_disabled_color(sf::Color);
    void set_text(std::string);

public:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    sf::RectangleShape shape;
    sf::Color not_clicked;
    sf::Color clicked;
    sf::Color disabled;
    sf::Text text;
    sf::Font font;

protected:
    bool enabled = true;
};


