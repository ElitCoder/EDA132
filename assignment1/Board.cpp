#include <iostream>
#include <string>

#include "Board.h"

Board::Board() {
    for(int8_t i = 0; i < 8; i++) {
        for(int8_t j = 0; j < 8; j++) {
            board[i][j] = ' ';
        }
    }

    (*this)["4d"] = (*this)["5e"] = 'W';
    (*this)["5d"] = (*this)["4e"] = 'B';
}

unsigned char& Board::operator[](const std::string &position) {
    int8_t x = toNumber(position[0]);
    int8_t y = toNumber(position[1]);

    return board[x][y];
}

void Board::printBoard() const {
    for(int8_t i = 0; i < 8; i++) {
        for(int8_t j = 0; j < 8; j++) {
            std::cout << '|' << board[i][j];
        }

        std::cout << "|\n";
    }
}

Position Board::putPiece(const std::string &position, const unsigned char c) {
    if(checkMove(position, c) == 0) {
        return Position::ILLEGAL_MOVE;
    }

    return Position::ILLEGAL_MOVE;
}

int8_t Board::checkMove(const std::string &position, const unsigned char c) {
    int x = toNumber(position[0]);
    int y = toNumber(position[1]);

    std::cout << "Testing " << position << " with color " << c << '\n';

    if((*this)[position] != ' ' || !insideBoard(x, y)) {
        return 0;
    }

    int8_t points = checkDirection(Direction::UP, x, y, c);
    points += checkDirection(Direction::DOWN, x, y, c);
    points += checkDirection(Direction::LEFT, x, y, c);
    points += checkDirection(Direction::RIGHT, x, y, c);
    points += checkDirection(Direction::DIAGONAL_SW, x, y, c);
    points += checkDirection(Direction::DIAGONAL_SE, x, y, c);
    points += checkDirection(Direction::DIAGONAL_NW, x, y, c);
    points += checkDirection(Direction::DIAGONAL_NE, x, y, c);

    return points;
}

int8_t Board::toNumber(const unsigned char c) const {
    if(c >= '1' && c <= '8') {
        return static_cast<int8_t>(c) - 49;
    }

    return static_cast<int8_t>(c) - 97;
}

bool Board::insideBoard(int8_t x, int8_t y) {
    return (x >= 1 && x <= 8 && y >= 1 && y <= 8);
}

const unsigned char Board::opponent(const unsigned char c) {
    switch(c) {
        case 'W': return 'B';
        case 'B': return 'W';
        default: return 'E';
    }

    return 'E';
}

int8_t Board::checkDirection(const Direction d, int8_t x, int8_t y, const unsigned char c) {
    if(!insideBoard(x, y)) {
        return 0;
    }

    const unsigned char op = opponent(c);
    bool foundSelf = false;
    int8_t points = 0;

    if(d == Direction::UP) {
        while(insideBoard(x, --y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    if(d == Direction::DOWN) {
        while(insideBoard(x, ++y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    if(d == Direction::LEFT) {
        while(insideBoard(--x, y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    if(d == Direction::RIGHT) {
        while(insideBoard(++x, y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    if(d == Direction::DIAGONAL_SW) {
        while(insideBoard(--x, ++y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    if(d == Direction::DIAGONAL_SE) {
        while(insideBoard(++x, ++y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    if(d == Direction::DIAGONAL_NW) {
        while(insideBoard(--x, --y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    if(d == Direction::DIAGONAL_NE) {
        while(insideBoard(++x, --y)) {
            if(board[x][y] == op) {
                points++;
            }

            else if(board[x][y] == c) {
                return points;
            }

            else if(board[x][y] == ' ') {
                return 0;
            }
        }
    }

    return foundSelf ? points : 0;
}
