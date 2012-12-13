#ifndef IA_H
#define IA_H

#include "list.h"
#include "vector.h"
#include "graph.h"
#include "generalGame.h"

namespace Gost
{
/*! \brief Le namespace IA englobe tout ce qui concerne les IA
 *
 */
namespace IA
{
    /*! \brief Cette fonction évalue si un déplacement est possible, auquel cas elle retourne le nouvel état créé
     *  \param currentState est l'état courant à étudier
     *  \param initialBoardNode est le noeud de la matrice de jeux d'où on part
     *  \param finalBoardNode est le noeud que l'on souhaite atteindre
     *  \param game est un référence vers le jeu
     */
    State* possibleMove(const State& currentState, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const GeneralGame &game);

    /*! \brief Cette fonction évalue les coups possibles par rapport à un état et une piece et les renvoie sous forme de liste
     *  \param currentState est l'état courant à étudier
     *  \param piece est l'élément dont on test déplacement
     *  \param game est une référence vers le jeu
     */
    List::Node<const State *>* getPossibleMove(const State& currentState, const Graph::Node* piece, const GeneralGame &game);

    /*! \brief Cette fonction évalue les coups possibles par rapport à un état et les renvoie sous forme de liste
     *  \param currentState est l'état courant à étudier
     *  \param game est une référence vers le jeu
     */
    List::Node<const State *>* getPossibleMove(const State& currentState, const GeneralGame &game);

    /*!
     * \brief Implémente l'algorithme A*
     * \param initialState L'état de départ.
     * \param finalState L'état final.
     * \param game Le jeu.
     * \return Une liste d'états correspondant à la résolution du jeu.
     */
    List::Node<const State *>* aStar(const State& initialState,const State& finalState, const GeneralGame &game);

    /*!
     * \brief Calcule le coût G d'un état.
     * \param currentState L'état dont on calcule le coût G.
     * \param initialState L'état initial.
     * \param finalState L'état final.
     * \param game Le jeu.
     * \return Le coût G de l'état.
     */
    unsigned int gScore(const State& currentState, const State& initialState, const State& finalState, const GeneralGame &game);

    /*!
     * \brief Calcule le coût H d'un état.
     * \param currentState L'état dont on calcule le coût H.
     * \param finalState L'état final.
     * \param game Le jeu.
     * \return Le coût H de l'état.
     */
    unsigned int hScore(const State& currentState, const State& finalState, const GeneralGame& game);

    /*! \brief Cette fonction évalue si on est à la fin du jeu et renvoie un booléen en conséquence
     *  \param currentState est l'état courant à étudier
     *  \param endState est l'état final à comparer
     *  \param game est un pointeur vers le jeu
     */
    inline bool isEnd(const State& currentState, const State& endState, const GeneralGame *game) {return hScore(currentState, endState, *game) == 0;}

}
}

#endif // IA_H
