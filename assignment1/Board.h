#ifndef BOARD_H
#define BOARD_H

enum class Position {
    ILLEGAL_MOVE
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    DIAGONAL_NE,
    DIAGONAL_NW,
    DIAGONAL_SE,
    DIAGONAL_SW
};

class Board {
public:
    Board();

    unsigned char& operator[](const std::string &position);

    void printBoard() const;
    Position putPiece(const std::string &position, const unsigned char c);
    int8_t checkMove(const std::string &position, const unsigned char c);
private:
    int8_t toNumber(const unsigned char c) const;
    bool insideBoard(int8_t x, int8_t y);
    const unsigned char opponent(const unsigned char c);
    int8_t checkDirection(const Direction d, int8_t x, int8_t y, const unsigned char c);

    unsigned char board[8][8];
};

#endif
