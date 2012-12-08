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
    //Matthieu je te laisserai faire des pointeurs de fonctions, pour moi c'est encore trop nébuleux ^^

    unsigned int nodeQuality(const State& currentState, const State& finalState, const Game &game);
}


#endif // IA_H
