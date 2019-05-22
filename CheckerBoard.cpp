#include"CheckerBoard.hpp"


Checker::Checker(Color color):color(color), type(SIMPLE)
{
    switch(color)
    {
        case WHITE:
            texture.loadFromFile("white.png");
            break;
        case BLACK:
            texture.loadFromFile("black.png");
            break;
        default:
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


Checker::Color Checker::getColor() const
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


bool CheckerBoard::deleteChecker(const BoardCoord coord)
{
    switch(m_board[coord.x][coord.y].status)
    {
        case BoardPiece::OCCUPIED:
            m_board[coord.x][coord.y].status = BoardPiece::FREE;
            m_board[coord.x][coord.y].ptrChecker.reset();
            return true;
        case BoardPiece::FREE:
           return false;
    }
}


boost::shared_ptr<Checker> CheckerBoard::getChecker(BoardCoord coord)
{
    return m_board[coord.x][coord.y].ptrChecker;
}


bool CheckerBoard::isChecker(const BoardCoord coord) const
{
    if(m_board[coord.x][coord.y].status == BoardPiece::OCCUPIED)
        return true;
    else
        return false;
}


Checker::Color getOppositeColor(Checker::Color color)
{
    switch(color)
    {
        case Checker::WHITE:
            return Checker::BLACK;
        case Checker::BLACK:
            return Checker::WHITE;
    }

}



static inline int m_abs(int x)
{
    if(x < 0 ) return -x;
    if(x > 0 ) return x;
    if(x == 0) return 0;
}


static inline int m_max(int x, int y)
{
    if(x < y ) return y;
    if(x > y ) return x;
    if(x == y) return x;
}


static inline int m_min(int x, int y)
{
    if(x < y ) return x;
    if(x > y ) return y;
    if(x == y) return x;
}


static inline int m_sign(int x)
{
    if(x < 0) return -1;
    if(x > 0 ) return 1;
    if(x == 0) return 0;
}


bool CheckerBoard::isThereRequerdTurnForChecker(BoardCoord oldCoord)
{
    if(!isChecker(oldCoord)) return false;

    if(getChecker(oldCoord)->getType() == Checker::SIMPLE)
    {
        boost::array<boost::array<int, 2>, 4> dXdY;

        dXdY[0][0] =  2; dXdY[0][1] =  2;
        dXdY[1][0] = -2; dXdY[1][1] = -2;
        dXdY[2][0] = -2; dXdY[2][1] =  2;
        dXdY[3][0] =  2; dXdY[3][1] = -2;

        int x = oldCoord.x;
        int y = oldCoord.y;

        BoardCoord newCoord;
        for(int i=0; i<dXdY.size(); ++i)
        {
            newCoord = BoardCoord(x + dXdY[i][0], y + dXdY[i][1]);
            if(newCoord.isValid())
            {
                if(isSimpleKillTurn(oldCoord, newCoord))
                {
                    return true;
                }
            }
        }
    }


    if(getChecker(oldCoord)->getType() == Checker::QUEEN)
    {
        std::cout<<"QUEEN"<<std::endl;

        boost::array<boost::array<int, 2>, 4> dXdY;

        dXdY[0][0] =  1; dXdY[0][1] =  1;
        dXdY[1][0] = -1; dXdY[1][1] = -1;
        dXdY[2][0] = -1; dXdY[2][1] =  1;
        dXdY[3][0] =  1; dXdY[3][1] = -1;

        Checker::Color playerColor = getChecker(oldCoord)->getColor();

        for(int i=0; i<dXdY.size(); ++i)
        {
            bool enemy = false;
            int x = oldCoord.x;
            int y = oldCoord.y;

            x += dXdY[i][0];
            y += dXdY[i][1];

            BoardCoord newCoord(x,y);
            while(newCoord.isValid())
            {
                if(isChecker(newCoord))
                {
                    if((getChecker(newCoord)->getColor() == playerColor)||enemy)
                    {
                        enemy = false;
                        break;
                    }
                    else
                        enemy = true;

                }
                else
                {
                    if(enemy)
                    {
                        return true;
                    }
                }

                newCoord.x += dXdY[i][0];
                newCoord.y += dXdY[i][1];
            }
        }
    }

    return false;
}


bool CheckerBoard::isThereRequiredTurns(Checker::Color playerColor)
{
    for(int i = 0; i < m_board.size(); ++i)
        for(int j = 0; j < m_board.size(); ++j)
        {
            if(isChecker(BoardCoord(i,j)))
            {
                if(getChecker(BoardCoord(i,j))->getColor() == playerColor)
                {
                    if(isThereRequerdTurnForChecker(BoardCoord(i,j)))
                        return true;
                }
            }

        }

    return false;
}



bool CheckerBoard::isKillTurn(BoardCoord oldCoord, BoardCoord newCoord)
{
    return isSimpleKillTurn(oldCoord, newCoord)||(isQueenKillTurn(oldCoord, newCoord));
}


bool CheckerBoard::isQueenTurn(BoardCoord oldCoord, BoardCoord newCoord)
{
        int dY = m_abs(newCoord.y-oldCoord.y);
        int dX = m_abs(newCoord.x-oldCoord.x);
        int sign_dX = m_sign(newCoord.x-oldCoord.x);
        int sign_dY = m_sign(newCoord.y-oldCoord.y);

        if(dX != dY) return false;

        int x = oldCoord.x;
        int y = oldCoord.y;

        for(int i=1; i < dX; ++i)
        {
            if(isChecker(BoardCoord::convertToBoardCoord(x+i*sign_dX, y+i*sign_dY)))
            {
                std::cout<<"invalidQueenTurn"<<std::endl;
                return false;
            }
        }

        return true;
}



bool CheckerBoard::isQueenKillTurn(BoardCoord oldCoord, BoardCoord newCoord)
{
        if(isChecker(newCoord)) return false;
        if(!oldCoord.isValid()||!newCoord.isValid()) return false;

        int dY = m_abs(newCoord.y-oldCoord.y);
        int dX = m_abs(newCoord.x-oldCoord.x);
        int sign_dX = m_sign(newCoord.x-oldCoord.x);
        int sign_dY = m_sign(newCoord.y-oldCoord.y);

        if(dX != dY) return false;

        int x = oldCoord.x;
        int y = oldCoord.y;

        boost::shared_ptr<Checker> active = getChecker(oldCoord);
        boost::shared_ptr<Checker> current;

        int countEnemy = 0;
        for(int i=1; i < dX; ++i)
        {
            if(isChecker(BoardCoord::convertToBoardCoord(x+i*sign_dX, y+i*sign_dY)))
            {
                std::cout<<"isChecker"<<x+i<<";"<<y+i<<std::endl;
                current = getChecker(BoardCoord::convertToBoardCoord(x+i*sign_dX, y+i*sign_dY));
                if(current->getColor() != active->getColor())
                    countEnemy++;
                else
                    return false;
            }
        }

        if(countEnemy != 1)
            return false;

        return true;
}


bool CheckerBoard::isSimpleKillTurn(BoardCoord oldCoord, BoardCoord newCoord)
{
        if(isChecker(newCoord)) return false;

        boost::shared_ptr<Checker> current = getChecker(oldCoord);

        int dY = newCoord.y-oldCoord.y;
        int dX = newCoord.x-oldCoord.x;
        int possible_dY;
        if(!(((dX == -2)||(dX == 2))&&(((dY == -2)||(dY == 2))))) return false;


        int enemyPos_X = (newCoord.x+oldCoord.x)/2;
        int enemyPos_Y = (newCoord.y+oldCoord.y)/2;
        if(!isChecker(BoardCoord(enemyPos_X,enemyPos_Y))) return false;

        boost::shared_ptr<Checker> enemy = getChecker(BoardCoord(enemyPos_X, enemyPos_Y));
        if(enemy->getColor() != getOppositeColor(current->getColor())) return false;

        return true;
}


bool CheckerBoard::isSimpleTurn(BoardCoord oldCoord, BoardCoord newCoord)
{
        int dY = newCoord.y-oldCoord.y;
        int dX = newCoord.x-oldCoord.x;
        int possible_dY;

        if(m_board[oldCoord.x][oldCoord.y].ptrChecker->getColor() == Checker::WHITE)
            possible_dY = -1;
        else
            possible_dY = 1;

        if((((dX == -1)||(dX == 1))&&((dY == possible_dY))))
            return true;

        return false;
}


CheckerBoard::TurnType CheckerBoard::getTurnType(BoardCoord oldCoord, BoardCoord newCoord)
{
    if(!(oldCoord.isValid()&&(newCoord.isValid()))) return INVALID_TURN;
    if( (newCoord.x + newCoord.y) % 2 != 1) return INVALID_TURN;
    if(m_board[newCoord.x][newCoord.y].status == BoardPiece::OCCUPIED ) return INVALID_TURN;


    boost::shared_ptr<Checker> active = getChecker(oldCoord);

    if(isThereRequiredTurns(active->getColor()))
    {
        if(active->getType() == Checker::SIMPLE)
        {
            if(isSimpleKillTurn(oldCoord, newCoord))
                return SIMPLE_KILL_TURN;
        }

        if(active->getType() == Checker::QUEEN)
        {
            if(isQueenKillTurn(oldCoord, newCoord))
                return QUEEN_KILL_TURN;
        }

        return INVALID_TURN;
    }
    else
    {
        if(active->getType() == Checker::SIMPLE)
        {
            if(isSimpleTurn(oldCoord, newCoord))
                return SIMPLE_TURN;
        }


        if(active->getType() == Checker::QUEEN)
        {
            if(isQueenTurn(oldCoord, newCoord))
                return QUEEN_TURN;
        }
    }

    return INVALID_TURN;
}


bool CheckerBoard::moveChecker(const BoardCoord oldPos, const BoardCoord newPos)
{
    boost::shared_ptr<Checker> activeChecker = getChecker(oldPos);

    if( (newPos.y == 0)&&(activeChecker->getColor() == Checker::WHITE) )
        activeChecker->makeQueen();


    if( (newPos.y == 7)&&(activeChecker->getColor() == Checker::BLACK) )
        activeChecker->makeQueen();

    activeChecker->setSpritePosition(newPos);

    m_board[newPos.x][newPos.y].ptrChecker.swap(activeChecker);
    m_board[oldPos.x][oldPos.y].status = BoardPiece::FREE;
    m_board[newPos.x][newPos.y].status = BoardPiece::OCCUPIED;
}


BoardCoord CheckerBoard::getCoordCheckerBetween(BoardCoord first, BoardCoord second)
{
    int dY = m_abs(second.y-first.y);
    int dX = m_abs(second.x-first.x);
    int sign_dX = m_sign(second.x-first.x);
    int sign_dY = m_sign(second.y-first.y);

    int x = first.x;
    int y = first.y;
    for(int i=1; i < dX; ++i)
    {
        if(isChecker(BoardCoord::convertToBoardCoord(x+i*sign_dX, y+i*sign_dY)))
            return BoardCoord::convertToBoardCoord(x+i*sign_dX, y+i*sign_dY);
    }

    return BoardCoord(-1,-1);
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
