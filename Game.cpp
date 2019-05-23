#include"Game.hpp"

void Game::draw(sf::RenderWindow& window)
{
    board.draw(window);

    if(isMove)activeChecker->draw(window);
}


void Game::startNewGame()
{
    isMove = false;
    requiredTurn = false;
    activePlayerColor = Checker::WHITE;
    gameOver = false;
    quantityBlackCheckers = 12;
    quantityWhiteCheckers = 12;

    board.freeBoard();
    board.initCheckers();
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
    if(gameOver) return;
    if(mousePos.y>=CELL_SIZE*BOARD_HEIGHT) return;

    BoardCoord coord = BoardCoord::convertToBoardCoord(mousePos);

    if(requiredTurn)
    {
        if( (coord.x == oldActiveCheckerPosition.x)&&(coord.y == oldActiveCheckerPosition.y) )
        {
            isMove = true;
        }
    }
    else if(board.isChecker(coord))
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
    if(gameOver) return;

    BoardCoord coord = BoardCoord::convertToBoardCoord(mousePos);

    if(isMove)
    {
        switch(board.getTurnType(oldActiveCheckerPosition, coord))
        {
            case CheckerBoard::INVALID_TURN:
                activeChecker->setSpritePosition(oldActiveCheckerPosition);
                break;
            case CheckerBoard::SIMPLE_TURN:
                board.moveChecker(oldActiveCheckerPosition, coord);
                passTurn();
                break;
            case CheckerBoard::QUEEN_TURN:
                board.moveChecker(oldActiveCheckerPosition, coord);
                passTurn();
                break;
            case CheckerBoard::SIMPLE_KILL_TURN:
                board.deleteChecker(board.getCoordCheckerBetween(oldActiveCheckerPosition, coord));
                decreaseQuantityCheckers();
                board.moveChecker(oldActiveCheckerPosition, coord);

                if(board.isThereRequerdTurnForChecker(coord))
                {
                    requiredTurn = true;
                    oldActiveCheckerPosition = coord;
                }
                else
                {
                    requiredTurn = false;
                    passTurn();
                }

                break;
            case CheckerBoard::QUEEN_KILL_TURN:
                board.deleteChecker(board.getCoordCheckerBetween(oldActiveCheckerPosition, coord));
                decreaseQuantityCheckers();
                board.moveChecker(oldActiveCheckerPosition, coord);

                if(board.isThereRequerdTurnForChecker(coord))
                {
                    requiredTurn = true;
                    oldActiveCheckerPosition = coord;
                }
                else
                {
                    requiredTurn = false;
                    passTurn();
                }
                break;
        }
    }

    isMove = false;
}


void Game::handleMouse(sf::Vector2i mousePos)
{
    if(gameOver) return;

    if(isMove)
    {
        activeChecker->setSpritePosition(mousePos);
    }
}


bool Game::isGameOver()
{
    return gameOver;
}


void Game::decreaseQuantityCheckers()
{
    switch(activePlayerColor)
    {
        case Checker::WHITE:
            --quantityBlackCheckers;
            break;
        case Checker::BLACK:
            --quantityWhiteCheckers;
            break;
    }

    if((quantityWhiteCheckers == 0)||(quantityBlackCheckers == 0))
        gameOver = true;
}
