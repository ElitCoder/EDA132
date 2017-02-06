#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <vector>

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
    Board(const Board &board);

    void print() const;

    char& board(const std::string& position);
    const char& board(const std::string& position) const;
    short put(const std::string &position, const char c);
    short put(const short x, const short y, const char c);
    short checkMove(const std::string &position, const char c) const;
    short checkMove(const short x, const short y, const char c) const;
    bool win(char &color) const;
    short countDistancePoints() const;
    char opponent(const char c) const;
    std::vector<std::array<short, 2>> getPossibleActions(const char playingColor) const;
    std::array<short, MAX_DIMENSIONS> currentPoints() const;

private:
    bool insideBoard(short x, short y) const;
    void turn(const short x, const short y, const char c);
    void turnDirection(const Direction direction, short x, short y, const char color);
    void changeDirection(const Direction direction, short &x, short &y) const;

    char countPoints() const;
    short checkDirection(const Direction d, short x, short y, const char c) const;
    std::array<short, MAX_DIMENSIONS> positionXY(const std::string &position) const;

    char m_board[8][8];
};

#endif
