#include <iostream>

#include "Board.h"

Board::Board() {
    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            m_board[i][j] = ' ';
        }
    }

    board("4d") = 'W';
    board("5e") = 'W';
    board("5d") = 'B';
    board("4e") = 'B';
}

char& Board::board(const std::string& position) {
    std::array<short, MAX_DIMENSIONS> matrixPosition = positionXY(position);

    return m_board[matrixPosition[0]][matrixPosition[1]];
}

void Board::print() const {
    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            std::cout << '|' << m_board[i][j];
        }

        std::cout << "|\n";
    }
}

short Board::put(const std::string &position, const char c) {
    if(checkMove(position, c) == 0) {
        return 0;
    }

    return 0;
}

std::array<short, MAX_DIMENSIONS> Board::positionXY(const std::string &position) const {
    return {position[0] - '1', position[1] - 'a'};
}

short Board::checkMove(const std::string &position, const char c) {
    std::array<short, MAX_DIMENSIONS> point = positionXY(position);
    short x = point[0], y = point[1];

    if(board(position) != ' ' || !insideBoard(x, y)) {
        return 0;
    }

    short points = checkDirection(Direction::UP, x, y, c);
    points += checkDirection(Direction::DOWN, x, y, c);
    points += checkDirection(Direction::LEFT, x, y, c);
    points += checkDirection(Direction::RIGHT, x, y, c);
    points += checkDirection(Direction::DIAGONAL_SW, x, y, c);
    points += checkDirection(Direction::DIAGONAL_SE, x, y, c);
    points += checkDirection(Direction::DIAGONAL_NW, x, y, c);
    points += checkDirection(Direction::DIAGONAL_NE, x, y, c);

    return points;
}

bool Board::insideBoard(short x, short y) const {
    return (x >= 0 && x < 8 && y >= 0 && y < 8);
}

const char Board::opponent(const char c) const {
    return c == 'W' ? 'B' : 'W';
}

short Board::checkDirection(const Direction direction, short xDirection, short yDirection, const char color) const {
    const char opponentColor = opponent(color);
    bool foundMatchingColor = false;
    short points = 0;

    for(short i = Direction::UP; i < Direction::MAX_DIRECTIONS; i++) {
        if(direction != i) {
            continue;
        }

        short x = xDirection;
        short y = yDirection;

        while(true) {
            switch(direction) {
                case Direction::UP: y--;
                    break;

                case Direction::DOWN: y++;
                    break;

                case Direction::LEFT: x--;
                    break;

                case Direction::RIGHT: x++;
                    break;

                case Direction::DIAGONAL_NE:
                    {
                        x++;
                        y--;
                    break;
                    }

                case Direction::DIAGONAL_NW:
                    {
                        x--;
                        y--;
                    break;
                    }

                case Direction::DIAGONAL_SE:
                    {
                        x++;
                        y++;
                    break;
                    }

                case Direction::DIAGONAL_SW:
                    {
                        x--;
                        y++;
                    break;
                    }
            }

            if(!insideBoard(x, y)) {
                break;
            }

            if(m_board[x][y] == opponentColor) {
                points++;
            }

            else if(m_board[x][y] == color) {
                return points;
            }

            else if(m_board[x][y] == ' ') {
                return 0;
            }
        }
    }

    return foundMatchingColor ? points : 0;
}
