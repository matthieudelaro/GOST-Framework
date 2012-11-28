#ifndef IA_H
#define IA_H

#include "vector.h"
#include "graph.h"
#include "game.h"

namespace IA
{
    const Vector<Graph::Node*>* possibleMove(const Vector<Graph::Node*>& currentStat, int pieceId, int direction, Game *game);
}


#endif // IA_H
