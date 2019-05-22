#ifndef __GAME__
#define __GAME__

#include <SFML/Graphics.hpp>
#include"CheckerBoard.hpp"

class Game
{
    private:
        CheckerBoard board;


        BoardCoord oldActiveCheckerPosition;
        boost::shared_ptr<Checker> activeChecker;
        bool isMove;
        bool requiredTurn;
        Checker::Color activePlayerColor;

    public:
        Game(){
            requiredTurn = false;
            activePlayerColor = Checker::WHITE;
            };

        void startGame();
        void passTurn();

        void draw(sf::RenderWindow&);

        void  pressedLeftMouse(sf::Vector2i);
        void releasedLeftMouse(sf::Vector2i);
        void handleMouse(sf::Vector2i);

};


#endif


