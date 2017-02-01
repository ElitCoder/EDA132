#ifndef AI_H
#define AI_H

#include "Board.h"

/*
struct State {
    char board[8][8];

    short black;
    short white;
}
*/

class AI {
public:
    void build();

    short terminalState(Board &board, const char playingColor);
    std::string minimaxDecision(Board &board);
    short maxValue(Board &board, const char playingColor);
    short minValue(Board &board, const char playingColor);
};

#endif
