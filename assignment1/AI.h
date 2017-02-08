#ifndef AI_H
#define AI_H

#include "Board.h"

#include <chrono>

enum MaxState {
    MAX_DEPTH = 12
};

enum class Minimax {
    MAX,
    MIN
};

class AI {
public:
    AI(const short maxTime);

    short terminalState(Board &board, short depth, Minimax type);
    std::array<short, 2> minimaxDecision(Board &board, const char playingColor);
    short maxValue(Board &board, const char playingColor, short depth, short alpha, short beta);
    short minValue(Board &board, const char playingColor, short depth, short alpha, short beta);
private:
    bool timeout();

    std::chrono::high_resolution_clock::time_point m_startClock;
    double m_maxTime;
};

#endif
