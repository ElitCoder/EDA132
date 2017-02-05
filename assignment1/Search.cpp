#include <iostream>

#include "AI.h"
#include "Board.h"

using action_vector_t = std::vector<std::array<short, 2>>;

void win(const char winner, const short white, const short black) {
    std::cout << "Points\nW: " << white << "\nB: " << black << "\n\n";

    if(winner == 'W') {
        std::cout << "Congratulations, you won!\n";
    }

    else {
        std::cout << "You lost, Skynet is next!\n";
    }
}

void printPoints(std::array<short, 2> &currentPoints) {
    std::cout << "      W: " << currentPoints[0] << " B: " << currentPoints[1] << '\n';
}

void game(AI &ai) {
    Board board;

    while(true) {
        auto currentPoints = board.currentPoints();
        printPoints(currentPoints);
        board.print();

        char winColor;
        bool winning = board.win(winColor);

        if(winning) {
            auto points = board.currentPoints();

            win(winColor, points[0], points[1]);

            break;
        }

        action_vector_t whiteActions = board.getPossibleActions('W');

        if(whiteActions.size() != 0) {
            short points(0);

            while(points == 0) {
                std::cout << "White (e.g \"6d\"): ";

                std::string input;
                std::cin >> input;

                points = board.put(input, 'W');

                if(points == 0) {
                    std::cout << "Illegal move, try again\n";

                    continue;
                }

                std::cout << "Well played, " << points << " point(s) awarded!\n\n";
            }

            currentPoints = board.currentPoints();
            printPoints(currentPoints);
            board.print();

            winning = board.win(winColor);

            if(winning) {
                auto points = board.currentPoints();

                win(winColor, points[0], points[1]);

                break;
            }
        }

        auto startClock = std::chrono::system_clock::now();
        std::array<short, MAX_DIMENSIONS> action = ai.minimaxDecision(board, 'B');
        std::chrono::duration<double, std::milli> duration = std::chrono::system_clock::now() - startClock;

        if(action[0] == -1 && action[1] == -1) {
            continue;
        }

        short points = board.put(action[0], action[1], 'B');

        if(points <= 0) {
            std::cout << "Error, AI calculated illegal move with (x, y) " << action[0] << ", " << action[1] << "\n\n";

            continue;
        }

        std::cout << "Computer played " << points << " point(s) (Used time: " << duration.count() / 1000 << "s)\n\n";
    }
}

int main() {
    std::cout << "Welcome to Othello (Reversi) VS AI!\nPlease enter an AI time limit (in seconds): ";

    short timeLimit;
    std::cin >> timeLimit;

    std::cout << '\n';

    AI ai(timeLimit);

    game(ai);

    return 0;
}
