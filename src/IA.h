#ifndef IA_H
#define IA_H

#include "vector.h"
#include "graph.h"
#include "game.h"

namespace IA
{
    State* possibleMove(const State& currentState, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const Game &game);
}


#endif // IA_H
