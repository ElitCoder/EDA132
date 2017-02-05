#include <iostream>
#include <vector>
#include <array>

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

Board::Board(const Board &board) {
    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            m_board[i][j] = board.m_board[i][j];
        }
    }
}

char& Board::board(const std::string& position) {
    std::array<short, MAX_DIMENSIONS> matrixPosition = positionXY(position);

    return m_board[matrixPosition[0]][matrixPosition[1]];
}

void Board::print() const {
    std::cout << "  ";

    for(char i = 'a'; i <= 'h'; i++) {
        std::cout << " " << i;
    }

    std::cout << '\n';

    for(short i = 0; i < 8; i++) {
        std::cout << (i + 1) << " ";

        for(short j = 0; j < 8; j++) {
            std::cout << '|' << m_board[j][i];
        }

        std::cout << "|\n";
    }

    std::cout << '\n';
}

short Board::put(const short x, const short y, const char c) {
    short points = checkMove(x, y, c);

    if(points == 0) {
        return 0;
    }

    turn(x, y, c);
    m_board[x][y] = c;

    return points;
}

short Board::put(const std::string &position, const char c) {
    std::array<short, MAX_DIMENSIONS> point = positionXY(position);
    short x = point[0], y = point[1];

    short points = checkMove(position, c);

    if(points == 0) {
        return 0;
    }

    turn(x, y, c);
    board(position) = c;

    return points;
}

std::array<short, MAX_DIMENSIONS> Board::positionXY(const std::string &position) const {
    return {position[1] - 'a', position[0] - '1'};
}

short Board::checkMove(const std::string &position, const char c) {
    std::array<short, MAX_DIMENSIONS> point = positionXY(position);
    short x = point[0], y = point[1];

    return checkMove(x, y, c);
}

short Board::checkMove(const short x, const short y, const char c) {
    if(m_board[x][y] != ' ' || !insideBoard(x, y)) {
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

short Board::countDistancePoints() {
    short whitePoints = 0, blackPoints = 0;

    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            if(m_board[j][i] == 'W') {
                whitePoints++;
            }

            else if(m_board[j][i] == 'B') {
                blackPoints++;
            }
        }
    }

    return blackPoints - whitePoints;
}

std::array<short, MAX_DIMENSIONS> Board::currentPoints() {
    std::array<short, MAX_DIMENSIONS> points = {0, 0};

    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            if(m_board[j][i] == 'W') {
                points[0]++;
            }

            else if(m_board[j][i] == 'B') {
                points[1]++;
            }
        }
    }

    return points;
}

char Board::countPoints() {
    short whitePoints = 0, blackPoints = 0;

    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            if(m_board[j][i] == 'W') {
                whitePoints++;
            }

            else if(m_board[j][i] == 'B') {
                blackPoints++;
            }
        }
    }

    return whitePoints > blackPoints ? 'W' : 'B';
}

bool Board::win(char &color) {
    short whitePoints = 0, blackPoints = 0;

    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            if(m_board[j][i] != ' ') {
                continue;
            }

            whitePoints += checkMove(j, i, 'W');
            blackPoints += checkMove(j, i, 'B');
        }
    }

    if(whitePoints == 0 && blackPoints == 0) {
        color = countPoints();
        return true;
    }

    return false;
}

void Board::turn(const short x, const short y, const char c) {
    turnDirection(Direction::UP, x, y, c);
    turnDirection(Direction::DOWN, x, y, c);
    turnDirection(Direction::LEFT, x, y, c);
    turnDirection(Direction::RIGHT, x, y, c);
    turnDirection(Direction::DIAGONAL_NE, x, y, c);
    turnDirection(Direction::DIAGONAL_NW, x, y, c);
    turnDirection(Direction::DIAGONAL_SE, x, y, c);
    turnDirection(Direction::DIAGONAL_SW, x, y, c);
}

void Board::changeDirection(const Direction direction, short &x, short &y) const {
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
}

void Board::turnDirection(const Direction direction, short x, short y, const char color) {
    std::vector<char*> turns;
    bool turnPieces(false);

    short xNew = x;
    short yNew = y;

    while(true) {
        changeDirection(direction, xNew, yNew);

        if(!insideBoard(xNew, yNew) || m_board[xNew][yNew] == ' ') {
            break;
        }

        if(m_board[xNew][yNew] == color) {
            turnPieces = true;

            break;
        }

        turns.push_back(&m_board[xNew][yNew]);
    }

    if(turnPieces) {
        for(char *piece : turns) {
            *piece = color;
        }
    }
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
            changeDirection(direction, x, y);

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

std::vector<std::array<short, 2>> Board::getPossibleActions(const char playingColor) {
    std::vector<std::array<short, 2>> actions;

    for(short i = 0; i < 8; i++) {
        for(short j = 0; j < 8; j++) {
            if(m_board[j][i] != ' ') {
                continue;
            }

            short points = checkMove(j, i, playingColor);

            if(points > 0) {
                actions.push_back({j, i});
            }
        }
    }

    return actions;
}
