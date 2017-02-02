#include <climits>
#include <vector>
#include <array>
#include <iostream>

#include "AI.h"

AI::AI(const short maxTime) : m_maxTime(maxTime) {
}

std::array<short, 2> AI::minimaxDecision(Board &original, const char playingColor) {
    std::vector<std::array<short, 2>> actions = original.getPossibleActions(playingColor);

    std::array<short, 2> bestAction;
    short bestPoints(SHRT_MIN);

    for(const std::array<short, 2> &action : actions) {
        Board board(original);
        short putPoints = board.put(action[0], action[1], playingColor);

        if(putPoints <= 0) {
            std::cout << "Error, the calculated action is illegal\n";

            continue;
        }

        short points = minValue(board, board.opponent(playingColor), 0);

        if(points > bestPoints) {
            bestAction = action;
            bestPoints = points;
        }
    }

    return bestAction;
}

short AI::terminalState(Board &board, const char playingColor, short depth) {
    char color;
    bool win(board.win(color));

    if(win || depth >= MAX_DEPTH) {
        return board.countDistancePoints(playingColor);
    }

    return SHRT_MIN;
}

short AI::maxValue(Board &board, const char playingColor, short depth) {
    short utility = terminalState(board, playingColor, depth);

    if(utility > SHRT_MIN) {
        return utility;
    }

    std::vector<std::array<short, 2>> actions = board.getPossibleActions(playingColor);

    std::array<short, 2> bestAction;
    short bestPoints(SHRT_MAX);

    for(const std::array<short, 2> &action : actions) {
        Board temp(board);
        short putPoints = temp.put(action[0], action[1], playingColor);

        if(putPoints <= 0) {
            std::cout << "Error, the calculated action is illegal\n";

            continue;
        }

        short points = minValue(temp, temp.opponent(playingColor), ++depth);

        if(points > bestPoints) {
            bestAction = action;
            bestPoints = points;
        }
    }
}

short AI::minValue(Board &board, const char playingColor, short depth) {
    short utility = terminalState(board, playingColor, depth);

    if(utility > SHRT_MIN) {
        return utility;
    }

    utility = SHRT_MAX;

    std::vector<std::array<short, 2>> actions = board.getPossibleActions(playingColor);

    std::array<short, 2> bestAction;
    short bestPoints(SHRT_MAX);

    for(const std::array<short, 2> &action : actions) {
        Board temp(board);
        short putPoints = temp.put(action[0], action[1], playingColor);

        if(putPoints <= 0) {
            std::cout << "Error, the calculated action is illegal\n";

            continue;
        }

        short points = maxValue(temp, temp.opponent(playingColor), ++depth);

        if(points < bestPoints) {
            bestAction = action;
            bestPoints = points;
        }
    }

    return bestPoints;
}
