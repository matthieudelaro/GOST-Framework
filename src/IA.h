#ifndef IA_H
#define IA_H

#include "list.h"
#include "vector.h"
#include "graph.h"
#include "game.h"

namespace IA
{
    /*! \brief Cette fonction évalue si un déplacement est possible, auquel cas elle retourne le nouvel état créé
     *  \param currentState est l'état courant à étudier
     *  \param initialBoardNode est le noeud de la matrice de jeux d'où on part
     *  \param finalBoardNode est le noeud que l'on souhaite atteindre
     *  \param game est un référence vers le jeu
     */
    State* possibleMove(const State& currentState, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const Game &game);

    /*! \brief Cette fonction évalue si on est à la fin du jeu et renvoie un booléen en conséquence
     *  \param currentState est l'état courant à étudier
     *  \param finalState est l'état final à comparer
     *  \param game est un pointeur vers le jeu
     */
    bool isEnd(const State& currentState, const State& endState, Game *game);

    /*! \brief Cette fonction évalue les coups possibles par rapport à un état et une piece et les renvoie sous forme de liste
     *  \param currentState est l'état courant à étudier
     *  \param piece est l'élément dont on test déplacement
     *  \param game est une référence vers le jeu
     */
    List::Node<const State *>* getPossibleMove(const State& currentState, const Graph::Node* piece, const Game &game);

    /*! \brief Cette fonction évalue les coups possibles par rapport à un état et les renvoie sous forme de liste
     *  \param currentState est l'état courant à étudier
     *  \param game est une référence vers le jeu
     */
    List::Node<const State *>* getPossibleMove(const State& currentState, const Game &game);

    List::Node<const State *>* aStar(const State& initialState,const State& finalState, const Game &game);

    //fonctions utiles pour le A*
    //unsigned int stateValue(const State& state, const Game &game);
    unsigned int gScore(const State& currentState, const State& initialState, const State& finalState, const Game &game);
    unsigned int hScore(const State& currentState, const State& finalState, const Game& game);
}


#endif // IA_H
