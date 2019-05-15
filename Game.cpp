#include"Game.hpp"

void Game::draw(sf::RenderWindow& window)
{
    board.draw(window);

    if(isMove)activeChecker->draw(window);
}


void Game::startGame()
{
    isMove = false;

    //разнести по функциям
    //___init__WHITE
    for(int i=0; i<3; ++i)
        for(int j=0; j<8; ++j)
        {
            if((i+j)%2==0)
                board.addChecker(new Checker(Checker::WHITE),j,7-i);
        }
    //__INIT_BLACK
    for(int i=0; i<3; ++i)
        for(int j=0; j<8; ++j)
        {
            if((i+j)%2==1)
                board.addChecker(new Checker(Checker::BLACK),j,i);
        }
}


void Game::passTurn()
{
    switch(activePlayerColor)
    {
        case Checker::WHITE:
            activePlayerColor= Checker::BLACK;
            break;
        case Checker::BLACK:
            activePlayerColor= Checker::WHITE;
            break;
    }
}


void Game::pressedLeftMouse(sf::Vector2i mousePos)
{
    BoardCoord coord = BoardCoord::convertToBoardCoord(mousePos);

    if(board.isChecker(coord))
    {
        activeChecker = board.getChecker(coord);

        if(activeChecker->getColor() == activePlayerColor)
        {
            isMove = true;
            oldActiveCheckerPosition = coord;
        }
    }
}


void Game::releasedLeftMouse(sf::Vector2i mousePos)
{
    BoardCoord coord = BoardCoord::convertToBoardCoord(mousePos);

    if(isMove)
    {
        switch(board.getTurnType(oldActiveCheckerPosition, coord))
        {
            case CheckerBoard::INVALID_TURN:
                std::cout<<"INVALID_TURN"<<std::endl;
                activeChecker->setSpritePosition(oldActiveCheckerPosition);
                break;
            case CheckerBoard::SIMPLE_TURN:
                std::cout<<"SIMPLE_TURN"<<std::endl;
                board.moveChecker(oldActiveCheckerPosition, coord);
                passTurn();
                break;
            /*case QUEEN_TURN:
                board.moveChecker(oldActiveCheckerPosition, coord);
                passTurn();
                break;
            case SIMPLE_KILL_TURN:
                board.deleteChecker(
                board.moveChecker(oldActiveCheckerPosition, coord);
                passTurn();
            case QUEEN_KILL_TURN:
            passTurn();*/
        }
    }

    isMove = false;
}


void Game::handleMouse(sf::Vector2i mousePos)
{
    if(isMove)
    {
        activeChecker->setSpritePosition(mousePos);
    }
}


