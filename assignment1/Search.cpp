#include <iostream>

#include "Board.h"

int main() {
    Board board;

    board.printBoard();
    std::cout << static_cast<int>(board.checkMove("6f", 'W')) << '\n';

    return 0;
}
