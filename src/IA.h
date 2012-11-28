#ifndef IA_H
#define IA_H

#include "vector.h"
#include "graph.h"
#include "game.h"

namespace IA
{
    const State* possibleMove(const State& currentStat, const Graph::Node* initPieceNode, const Graph::Node* finalPieceNode, const Game &game);
}


#endif // IA_H
