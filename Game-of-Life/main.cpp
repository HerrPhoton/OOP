#include <SFML/Graphics.hpp>
#include "Game.h"

constexpr int win_height = 900;
constexpr int win_width = win_height + 500;
constexpr int offset = 10;

int main() {

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Game of Life",
		sf::Style::Titlebar | sf::Style::Close);

	window.setFramerateLimit(60);

	sf::Event event;

	Game game(win_width, win_height, offset);
	game.setPosition(offset, offset);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			game.get_event(event);

			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonReleased:

				if (!game.is_work())
					game.reset();

				game.reset_buttons();

				break;

			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape)
					window.close();

				break;
			}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i pos = sf::Mouse::getPosition(window);
				game.mouse_clicked(pos);
			}	
		}

		if (game.is_work())
			game.game_step();

		window.clear();
		window.draw(game);
		window.display();
	}

	return 0;
}