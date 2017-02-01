#include <climits>

#include "AI.h"

void AI::build() {
}

std::string AI::minimaxDecision(Board &board) {
    std::string position("");

    return position;
}

short AI::terminalState(Board &board, const char playingColor) {
    char color;
    bool win(board.win(color));

    if(win) {
        return board.countDistancePoints(playingColor);
    }

    return SHRT_MIN;
}

short AI::maxValue(Board &board, const char playingColor) {
    short utility = terminalState(board, playingColor);

    if(utility > SHRT_MIN) {
        return utility;
    }


}

short AI::minValue(Board &board, const char playingColor) {
    short utility = terminalState(board, playingColor);

    if(utility > SHRT_MIN) {
        return utility;
    }

    utility = SHRT_MAX;
}
