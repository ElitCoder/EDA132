#include <iostream>

#include "Board.h"

int main() {
    Board board;

    board.print();
    
    std::cout << board.checkMove("5f", 'B') << '\n';

    return 0;
}
