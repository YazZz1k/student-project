#ifndef _CHECKER_BOARD_
#define _CHECKER_BOARD_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

enum{BOARD_WIDTH = 8, BOARD_HEIGHT = 8, CELL_SIZE = 50, OFFSET = 30};


class BoardCoord
{
    public:
        int x, y;

    public:
        BoardCoord(){}
        BoardCoord(int x, int y):x(x), y(y){}
        ~BoardCoord(){}
        BoardCoord(const BoardCoord& boardCoord)
        {
            x = boardCoord.x;
            y = boardCoord.y;
        }

        BoardCoord& operator=(const BoardCoord& coord)
        {
            x = coord.x;
            y = coord.y;

            return *this;
        }

        static BoardCoord convertToBoardCoord(int x, int y)
        {
            return BoardCoord(x,y);
        }

        static BoardCoord convertToBoardCoord(sf::Vector2i pixelPos)
        {
            return BoardCoord(pixelPos.x/CELL_SIZE, pixelPos.y/CELL_SIZE);
        }

        bool isValid() const
        {
            return !((x>7)||(x<0)||(y>7)||(y<0));
        }
};


class Checker
{
    public:
        enum Color {WHITE, BLACK};
        enum CheckerType  {SIMPLE, QUEEN};
    private:
        Color color;
        CheckerType  type;
        sf::Sprite sprite;
        sf::Texture texture;

    public:
        explicit Checker(Color color);
        ~Checker(){}

        void makeQueen();
        CheckerType getType()const;
        void setSpritePosition(const BoardCoord& coord);
        void setSpritePosition(sf::Vector2i newPosition);
        void draw(sf::RenderWindow& window);

        Color getColor()const;
    private:
        Checker(const Checker&)        = delete;
        void operator=(const Checker&) = delete;
};


class CheckerBoard
{
    public:
        enum TurnType { SIMPLE_TURN, INVALID_TURN, QUEEN_TURN, SIMPLE_KILL_TURN, QUEEN_KILL_TURN};
        struct BoardPiece
        {
            enum Status {OCCUPIED, FREE};
            boost::shared_ptr<Checker> ptrChecker;
            sf::Sprite sprite;
            Status status = FREE;
       };

    public:
        typedef boost::array<boost::array<BoardPiece, BOARD_WIDTH>, BOARD_HEIGHT> BoardType;

    private:
        BoardType m_board;

    public:
        CheckerBoard();
        CheckerBoard(const CheckerBoard&)   = delete;
        void operator=(const CheckerBoard&) = delete;
        ~CheckerBoard(){};

        void draw(sf::RenderWindow&);
        bool addChecker(Checker* added, int x, int y);
        boost::shared_ptr<Checker> getChecker(BoardCoord);

        bool isChecker(const BoardCoord)const;
        bool moveChecker(const BoardCoord, const BoardCoord);
        bool deleteChecker(const BoardCoord);
        BoardCoord getCoordCheckerBetween(BoardCoord, BoardCoord);

        bool isThereRequerdTurnForChecker(BoardCoord);
        TurnType getTurnType(BoardCoord, BoardCoord);

        void freeBoard();
        void initCheckers();
    private:
        void _initilizate();

        bool isThereRequiredTurns(Checker::Color);
        bool isKillTurn(BoardCoord, BoardCoord);


        bool isSimpleTurn(BoardCoord, BoardCoord);
        bool isSimpleKillTurn(BoardCoord, BoardCoord);

        bool isQueenTurn(BoardCoord, BoardCoord);
        bool isQueenKillTurn(BoardCoord, BoardCoord);
};


#endif
