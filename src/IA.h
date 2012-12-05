#ifndef IA_H
#define IA_H

#include "list.h"
#include "vector.h"
#include "graph.h"
#include "game.h"

namespace IA
{
    State* possibleMove(const State& currentState, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const Game &game);
    bool isEnd(const State& currentState, const State& endState, Game *game);
    List::Node<const State *>* getPossibleMove(const State& currentState, const Graph::Node* piece, const Game &game);
    List::Node<const State *>* getPossibleMove(const State& currentState, const Game &game);
}


#endif // IA_H
