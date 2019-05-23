#ifndef __MENU__
#define __MENU__

#include"CheckerBoard.hpp"
#include <SFML/Graphics.hpp>

class Menu
{
    public:
        enum Command {EXIT, RESET, NOTHING};
    private:
        sf::Texture textureResetBottom;
        sf::Texture textureExitBottom;

        sf::Sprite spriteResetBottom;
        sf::Sprite spriteExitBottom;
        sf::Sprite background;

        bool exit;
        bool reset;

        bool isExitSpriteActive;
        bool isResetSpriteActive;
    public:

        Menu();

        void pressedLeftMouse(sf::Vector2i pixelPos);
        void releasedLeftMouse(sf::Vector2i pixelPos);
        Command getCommand();
        void draw(sf::RenderWindow& window);
};

#endif
