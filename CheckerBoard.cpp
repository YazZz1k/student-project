#include"CheckerBoard.hpp"


Checker::Checker(CheckerColor color):color(color), type(SIMPLE)
{
    //sf::Texture texture;
    switch(color)
    {
        case WHITE:
            texture.loadFromFile("white.png");
            break;
        case BLACK:
            texture.loadFromFile("black.png");
            break;
        default:
            std::cout<<"ERROR CHECKER CONSTRUCTOR"<<std::endl;
            break;
    }

    sprite.setTexture(texture);
    sprite.setScale(CELL_SIZE/sprite.getLocalBounds().width,CELL_SIZE/sprite.getLocalBounds().height);
}

void Checker::makeQueen()
{
    type = QUEEN;
    switch(color)
    {
        case WHITE:
            texture.loadFromFile("whiteQueen.png");
            break;
        case BLACK:
            texture.loadFromFile("blackQueen.png");
            break;
        default:
            std::cout<<"ERROR CHECKER CONSTRUCTOR"<<std::endl;
            break;
    }

    sprite.setTexture(texture);
    sprite.setScale(CELL_SIZE/sprite.getLocalBounds().width,CELL_SIZE/sprite.getLocalBounds().height);
}


void Checker::setSpritePosition(const BoardCoord& coord)
{
    if(coord.isValid())
        sprite.setPosition(CELL_SIZE*coord.x, CELL_SIZE*coord.y);
}


Checker::CheckerColor Checker::getColor() const
{
    return color;
}


void Checker::setSpritePosition(sf::Vector2i pixelPos)
{
    int x = pixelPos.x;
    int y = pixelPos.y;

    if(x>=CELL_SIZE*BOARD_WIDTH-CELL_SIZE/2)
        x = CELL_SIZE*BOARD_WIDTH-CELL_SIZE/2;

    if(y>=CELL_SIZE*BOARD_WIDTH-CELL_SIZE/2)
        y = CELL_SIZE*BOARD_WIDTH-CELL_SIZE/2;

    if(x<=CELL_SIZE/2)
        x = CELL_SIZE/2;

    if(y<=CELL_SIZE/2)
        y = CELL_SIZE/2;

    sprite.setPosition(x-CELL_SIZE/2, y-CELL_SIZE/2);
}

Checker::CheckerType Checker::getType()const
{
    return type;
}

void Checker::draw(sf::RenderWindow& window)
{
    window.draw(sprite);
}


CheckerBoard::CheckerBoard()
{
    _initilizate();
}

void CheckerBoard::draw(sf::RenderWindow& window)
{
    for(int i=0; i<m_board.size(); ++i)
        for(int j=0; j<m_board.size(); ++j)
            window.draw(m_board[i][j].sprite);

    //draw checkers
    for(int i=0; i<m_board.size(); ++i)
        for(int j=0; j<m_board.size(); ++j)
        {
            if(m_board[i][j].status == BoardPiece::OCCUPIED)
                m_board[i][j].ptrChecker->draw(window);
        }

}

bool CheckerBoard::addChecker(Checker* added, int x, int y)
{
    switch(m_board[x][y].status)
    {
        case BoardPiece::OCCUPIED:
            return false;
        case BoardPiece::FREE:
            m_board[x][y].status = BoardPiece::OCCUPIED;
            m_board[x][y].ptrChecker = boost::shared_ptr<Checker>(added);
            m_board[x][y].ptrChecker->setSpritePosition(BoardCoord::convertToBoardCoord(x,y));
            return true;
    }
}

boost::shared_ptr<Checker> CheckerBoard::getChecker(BoardCoord coord)
{
    return m_board[coord.x][coord.y].ptrChecker;
}


bool CheckerBoard::isChecker(BoardCoord coord)
{
    if(m_board[coord.x][coord.y].status == BoardPiece::OCCUPIED)
        return true;
    else
        return false;
}


CheckerBoard::TurnType CheckerBoard::getTurnType(BoardCoord oldCoord, BoardCoord newCoord)
{
    if(!(oldCoord.isValid()&&(newCoord.isValid()))) return INVALID_TURN;
    if( (newCoord.x + newCoord.y) % 2 != 1) return INVALID_TURN;
    if(m_board[newCoord.x][newCoord.y].status == BoardPiece::OCCUPIED ) return INVALID_TURN;


    if(m_board[oldCoord.x][oldCoord.y].ptrChecker->getType() == Checker::SIMPLE)
    {
        int dX = newCoord.x-oldCoord.x;
        int dY = newCoord.y-oldCoord.y;

        int possible_dY;
        if(m_board[oldCoord.x][oldCoord.y].ptrChecker->getColor() == Checker::WHITE)
        {
            possible_dY = -1;
        }
        else
        {
            possible_dY = 1;
        }

        if((((dX == -1)||(dX == 1))&&((dY == possible_dY))))
        {
            return SIMPLE_TURN;
        }
    }

    return INVALID_TURN;



}
bool CheckerBoard::isValidTurn(BoardCoord oldCoord, BoardCoord newCoord)
{
    if(!(oldCoord.isValid()&&(newCoord.isValid())))
    {
        std::cout<<"Ходить нельзя:невалидные коорд"<<std::endl;
        return false;
    }

    if((newCoord.x + newCoord.y ) % 2 != 1)
    {
        std::cout<<"Ходить нельзя:нельзя ставить на белую клетку"<<std::endl;
        return false;
    }

    BoardPiece::Status statusNewCoord = m_board[newCoord.x][newCoord.y].status;
    if(statusNewCoord == BoardPiece::OCCUPIED)
    {
        std::cout<<"занято"<<std::endl;
        return false;
    }


    if(m_board[oldCoord.x][oldCoord.y].ptrChecker->getType() == Checker::SIMPLE)
    {
        int dX = newCoord.x-oldCoord.x;
        int dY = newCoord.y-oldCoord.y;

        //Simple Turn
        int possible_dY = 0;
        if(m_board[oldCoord.x][oldCoord.y].ptrChecker->getColor() == Checker::WHITE)
        {
            possible_dY = -1;
        }
        else
        {
            possible_dY = 1;
        }

        if(!(((dX == -1)||(dX == 1))&&((dY == possible_dY))))
        {
            return true;
        }

        //kill turn
        possible_dY = 0;
        if(m_board[oldCoord.x][oldCoord.y].ptrChecker->getColor() == Checker::WHITE)
        {
            possible_dY = -2;
        }
        else
        {
            possible_dY = 2;
        }


        if((((dX == -2)||(dX == 2))&&((dY == possible_dY))))
        {
            if(m_board[oldCoord.x+dX/2][oldCoord.y+dY/2].ptrChecker->getColor() == Checker::BLACK)
                return true;
        }
    }


    if(m_board[oldCoord.x][oldCoord.y].ptrChecker->getType() == Checker::QUEEN)
    {
        int dX = newCoord.x-oldCoord.x;
        int dY = newCoord.y-oldCoord.y;

        if(dX == dY)
            return true;

        if(dX == -dY)
            return true;
    }

    return true;
}


void CheckerBoard::doTurn(BoardCoord oldPos, BoardCoord newPos)
{
    boost::shared_ptr<Checker> activeChecker = getChecker(oldPos);
    activeChecker->setSpritePosition(newPos);
    m_board[newPos.x][newPos.y].ptrChecker.swap(activeChecker);
    m_board[oldPos.x][oldPos.y].status = BoardPiece::FREE;
    m_board[newPos.x][newPos.y].status = BoardPiece::OCCUPIED;

    if(newPos.y == 0)
    {
        m_board[newPos.x][newPos.y].ptrChecker->makeQueen();
    }
}


bool CheckerBoard::moveChecker(BoardCoord oldPos, BoardCoord newPos)
{
    boost::shared_ptr<Checker> activeChecker = getChecker(oldPos);
    activeChecker->setSpritePosition(newPos);
    m_board[newPos.x][newPos.y].ptrChecker.swap(activeChecker);
    m_board[oldPos.x][oldPos.y].status = BoardPiece::FREE;
    m_board[newPos.x][newPos.y].status = BoardPiece::OCCUPIED;

    if(newPos.y == 0)
    {
        m_board[newPos.x][newPos.y].ptrChecker->makeQueen();
    }
}




void CheckerBoard::_initilizate()
{
    for(int i = 0; i < m_board.size(); ++i)
    {
        for(int j = 0; j < m_board.size(); ++j)
        {
            sf::Texture texture;
            texture.create(CELL_SIZE, CELL_SIZE);
            m_board[i][j].sprite.setTexture(texture);

            if( (i + j) % 2 == 0)
            {
                m_board[i][j].sprite.setColor(sf::Color(50,50,50));
            }
            else
            {
                m_board[i][j].sprite.setColor(sf::Color(100,100,100));
            }

            m_board[i][j].sprite.setPosition(CELL_SIZE*j, CELL_SIZE*i);
        }
    }
}
