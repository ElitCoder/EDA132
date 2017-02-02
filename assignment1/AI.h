#ifndef AI_H
#define AI_H

#include "Board.h"

enum MaxState {
    MAX_DEPTH = 10
};

class AI {
public:
    AI(const short maxTime);

    short terminalState(Board &board, const char playingColor, short depth);
    std::array<short, 2> minimaxDecision(Board &board, const char playingColor);
    short maxValue(Board &board, const char playingColor, short depth);
    short minValue(Board &board, const char playingColor, short depth);
private:
    short m_maxTime;
};

#endif
