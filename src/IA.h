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

    List::Node<const State *>* aStar(const State *initialState,const State *finalState, const Game &game);

    //fonctions utiles pour le A*
    unsigned int stateValue(const State& state, const Game &game);
    unsigned int gScore(const State& currentState, const State& initialState, const State& finalState, const Game &game);
    unsigned int hScore(const State& currentState, const State& initialState, const State& finalState, const Game &game);
}


#endif // IA_H
