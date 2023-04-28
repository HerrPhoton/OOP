#pragma once

#include <SFML/Graphics.hpp>

class TextBox : public sf::Drawable {

public:
    TextBox();
    ~TextBox() = default;

public:
    bool check_click(sf::Vector2i);

public:
    void set_size(sf::Vector2f);
    void set_pos(sf::Vector2f);
    void set_text(std::string);
    void set_active_color(sf::Color);
    void set_inactive_color(sf::Color);
    void change_color(bool);
    void add_char(char);
    void delete_char();
    void clear_text();
    std::string get_text();
    
public:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
    sf::RectangleShape shape;
    sf::Color active_color;
    sf::Color inactive_color;
    sf::Text text;
    sf::Font font;

private:
    bool active = false;
    int shrinked_cnt = 0;
};


