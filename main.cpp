#include"Game.hpp"
#include"Menu.hpp"
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE*BOARD_WIDTH, CELL_SIZE*BOARD_HEIGHT+OFFSET), "Checkers", sf::Style::Close);
    window.setFramerateLimit(60);

    Game game;
    Menu menu;

    game.startNewGame();

    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Event event;

        game.handleMouse(pixelPos);

        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::MouseButtonPressed:
                    if(event.key.code == sf::Mouse::Left)
                    {
                        game.pressedLeftMouse(pixelPos);
                        menu.pressedLeftMouse(pixelPos);
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if(event.key.code == sf::Mouse::Left)
                    {
                        game.releasedLeftMouse(pixelPos);
                        menu.releasedLeftMouse(pixelPos);
                    }
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
            }

        }

        window.clear();
        game.draw(window);

        menu.draw(window);

        switch(menu.getCommand())
        {
            case Menu::EXIT:
                window.close();
                break;
            case Menu::RESET:
                game.startNewGame();
                break;
            default:
                break;
        }

        window.display();
    }
    return 0;
}
