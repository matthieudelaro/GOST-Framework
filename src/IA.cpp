#include <QDebug>
#include "IA.h"

const State* IA::possibleMove(const State& currentStat, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const Game &game)
{
    qDebug() << "nouveau deplacement";

    //vérification que les deux noeuds sont ajacents

    if(initialBoardNode == NULL || finalBoardNode == NULL)
    {
        qDebug() << "la case n'existe pas";
        return NULL ;
    }

    //on regarde si les deux liens sont bien voisins et on récupère la direction
    int direction = -1;
    for(int i = 0; i < initialBoardNode->nbLinks; i ++)
    {
        if(initialBoardNode->getConstLink(i) == finalBoardNode)
            direction = i;
    }
    if(direction == -1)
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

