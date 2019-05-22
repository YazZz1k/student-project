#include"Game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(8*50, 8*50), "Checkers", sf::Style::Close);
    window.setFramerateLimit(60);

    Game game;

    game.startGame();

    while (window.isOpen())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Event event;

        game.handleMouse(pixelPos);

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::MouseButtonPressed:
                    if(event.key.code == sf::Mouse::Left)
                    {
                        game.pressedLeftMouse(pixelPos);
                    }
                    break;
                case sf::Event::MouseButtonReleased:
                    if(event.key.code == sf::Mouse::Left)
                    {
                        game.releasedLeftMouse(pixelPos);
                    }
                    break;
                case sf::Event::Closed:
                    window.close();
                    break;
            }

        }

        window.clear();
        game.draw(window);
        window.display();
    }
    return 0;
}
