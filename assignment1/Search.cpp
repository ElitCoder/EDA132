#include <iostream>

#include "AI.h"
#include "Board.h"

void game(AI &ai) {
    Board board;

    char turn = 'W';

    while(true) {
        board.print();

        short points(0);

        while(points == 0) {
            if(turn == 'W') {
                std::cout << "White ";
            }

            else {
                std::cout << "Black ";
            }

            std::cout << "(e.g \"6d\"): ";

            std::string input;
            std::cin >> input;

            points = board.put(input, turn);

            if(points == 0) {
                std::cout << "Illegal move, try again\n";

                continue;
            }

            std::cout << "Well played, " << points << " point(s) awarded!\n\n";

            turn = board.opponent(turn);
        }
    }
}

int main() {
    AI ai;
    ai.build();

    game(ai);

    return 0;
}
