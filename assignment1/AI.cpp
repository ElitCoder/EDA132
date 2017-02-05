#include "AI.h"

#include <climits>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <chrono>

AI::AI(const short maxTime) : m_maxTime(maxTime * 1000) {
}

std::array<short, 2> AI::minimaxDecision(Board &original, const char playingColor) {
    m_startClock = std::chrono::system_clock::now();

    std::vector<std::array<short, 2>> actions = original.getPossibleActions(playingColor);

    if(actions.size() == 0) {
        return {-1, -1};
    }

    std::array<short, 2> bestAction;
    short bestPoints(SHRT_MIN);

    for(const std::array<short, 2> &action : actions) {
        Board board(original);
        short putPoints = board.put(action[0], action[1], playingColor);

        if(putPoints <= 0) {
            std::cout << "Error, the calculated action is illegal\n";

            continue;
        }

        short points = minValue(board, board.opponent(playingColor), 0, SHRT_MIN, SHRT_MAX);

        if(points > bestPoints) {
            bestAction = action;
            bestPoints = points;
        }
    }

    return bestAction;
}

bool AI::timeout() {
    std::chrono::duration<double, std::milli> duration = std::chrono::system_clock::now() - m_startClock;

    if(duration.count() >= m_maxTime) {
        return true;
    }

    return false;
}

short AI::terminalState(Board &board, const char playingColor, short depth) {
    char color;
    bool win(board.win(color));

    if(win || depth >= MAX_DEPTH || timeout()) {
        return board.countDistancePoints();
    }

    return SHRT_MIN;
}

short AI::maxValue(Board &board, const char playingColor, short depth, short alpha, short beta) {
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

        short points = minValue(temp, temp.opponent(playingColor), ++depth, alpha, beta);

        short distancePoints = temp.countDistancePoints();

        if(points > bestPoints) {
            bestAction = action;
            bestPoints = distancePoints;
        }

        if(distancePoints >= beta) {
            return distancePoints;
        }

        alpha = std::max(alpha, distancePoints);
    }

    return bestPoints;
}

short AI::minValue(Board &board, const char playingColor, short depth, short alpha, short beta) {
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

        short points = maxValue(temp, temp.opponent(playingColor), ++depth, alpha, beta);

        short distancePoints = temp.countDistancePoints();

        if(distancePoints < bestPoints) {
            bestAction = action;
            bestPoints = distancePoints;
        }

        if(distancePoints <= alpha) {
            return distancePoints;
        }

        beta = std::min(beta, distancePoints);
    }

    return bestPoints;
}
