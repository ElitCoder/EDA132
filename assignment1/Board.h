#ifndef BOARD_H
#define BOARD_H

#include <array>

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DIAGONAL_NE,
    DIAGONAL_NW,
    DIAGONAL_SE,
    DIAGONAL_SW,
    MAX_DIRECTIONS
};

enum Point {
    X,
    Y,
    MAX_DIMENSIONS
};

class Board {
public:
    Board();

    void print() const;

    char& board(const std::string& position);
    short put(const std::string &position, const char c);
    short checkMove(const std::string &position, const char c);
    short checkMove(const short x, const short y, const char c);
    bool win(char &color);
    short countDistancePoints(char color);
    const char opponent(const char c) const;
private:
    bool insideBoard(short x, short y) const;
    void turn(const std::string &position, const char c);
    bool turnDirection(const Direction direction, short x, short y, const char color);
    void changeDirection(const Direction direction, short &x, short &y) const;

    char countPoints();
    short checkDirection(const Direction d, short x, short y, const char c) const;
    std::array<short, MAX_DIMENSIONS> positionXY(const std::string &position) const;

    char m_board[8][8];
};

#endif
