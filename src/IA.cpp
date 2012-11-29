#include <QDebug>
#include "IA.h"

const State* IA::possibleMove(const State& currentStat, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const Game &game)
{
    qDebug() << "nouveau deplacement";

    //vérification que les deux noeuds sont ajacents

    if(initialBoardNode == NULL || finalBoardNode == NULL) return NULL ; //si on va ou on vient d'une case qui n'existe pas

    //on regarde si les deux liens sont bien voisins
    bool isNeighbour = false;
    for(int i = 0; i < initialBoardNode->nbLinks; i ++)
    {
        if(initialBoardNode->getConstLink(i) == finalBoardNode)
            isNeighbour = true;
    }
    if(!isNeighbour)
    {
        qDebug() << "ce ne sont pas des voisins";
        return NULL;
    }

    //on vérifie que l'on ne prend pas une case vide
    if(game.getPieceNode(initialBoardNode, currentStat) == NULL)
    {
        qDebug() << "ce n'est pas un pièce";
        return NULL;
    }

    return NULL;
}

