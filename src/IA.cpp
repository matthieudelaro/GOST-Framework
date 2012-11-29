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
    for(unsigned int i = 0; i < initialBoardNode->nbLinks; i ++)
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
        qDebug() << "ce n'est pas un piece";
        return NULL;
    }

    //ATTENTION : TOLIST NE PREND PAS DE CONST
    List::Node<const Graph::Node*>* pieceToMove = Graph::toConstList(game.getPieceNode(initialBoardNode, currentStat));

    //on parcours toute les sous-pièces
    while(pieceToMove != NULL)
    {
        //on test si la direction est bonne : si on ne sort pas du terrain
        if(pieceToMove->info->getConstLink(direction) == NULL)
        {
            qDebug() << "on sort du terrain";
            return NULL;
        }

        //pour cette sous-pièce c'est bon, on passe à la suivante
        pieceToMove = pieceToMove->next;
    }

    return NULL;
}

