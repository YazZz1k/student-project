#include"Menu.hpp"


Menu::Menu()
{
    exit = false;
    reset = false;

    isExitSpriteActive = false;
    isResetSpriteActive = false;

    textureResetBottom.loadFromFile("restart.png");
    textureExitBottom.loadFromFile("exit.png");

    spriteResetBottom.setTexture(textureResetBottom);
    spriteExitBottom.setTexture(textureExitBottom);

    spriteResetBottom.setScale(OFFSET/spriteResetBottom.getLocalBounds().width,OFFSET/spriteResetBottom.getLocalBounds().height);
    spriteExitBottom.setScale(OFFSET/spriteExitBottom.getLocalBounds().width,OFFSET/spriteExitBottom.getLocalBounds().height);

    spriteResetBottom.setPosition(0,CELL_SIZE*BOARD_HEIGHT);
    spriteExitBottom.setPosition(OFFSET, CELL_SIZE*BOARD_HEIGHT);

    sf::Texture textureBackground;
    textureBackground.create(CELL_SIZE*BOARD_WIDTH, OFFSET);
    background.setTexture(textureBackground);
    background.setColor(sf::Color::White);
    background.setPosition(0,CELL_SIZE*BOARD_HEIGHT);
}


void Menu::pressedLeftMouse(sf::Vector2i pixelPos)
{
    if(spriteResetBottom.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
    {
        isResetSpriteActive = true;
    }

    if(spriteExitBottom.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
    {
        isExitSpriteActive = true;
    }
}


void Menu::releasedLeftMouse(sf::Vector2i pixelPos)
{
    if(spriteResetBottom.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
    {
        if(isResetSpriteActive)
            reset = true;
    }

    if(spriteExitBottom.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
    {
        if(isExitSpriteActive)
            exit = true;
    }
}


Menu::Command Menu::getCommand()
{
    if(reset)
    {
        reset = false;
        return RESET;
    }

    if(exit)
    {
        exit = false;
        return EXIT;
    }

    return NOTHING;
}


void Menu::draw(sf::RenderWindow& window)
{
    window.draw(background);
    window.draw(spriteResetBottom);
    window.draw(spriteExitBottom);
}

