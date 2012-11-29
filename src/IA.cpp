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

    qDebug() << "la case existe";

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

    qDebug() << "ce sont des voisins";

    //on vérifie que l'on ne prend pas une case vide
    if(game.getPieceNode(initialBoardNode, currentStat) == NULL)
    {
        qDebug() << "ce n'est pas un piece";
        return NULL;
    }

    qDebug() << "c'est une piece";

    List::Node<const Graph::Node*>* pieceToMove = Graph::toConstList(game.getPieceNode(initialBoardNode, currentStat));
    List::Node<const Graph::Node*>* iterator = pieceToMove;

    //on parcours toute les sous-pièces
    while(iterator != NULL)
    {
        //on test si la direction est bonne : si on ne sort pas du terrain
        //BUG ICI
        if(game.getPieceNode(iterator->info->getLink(direction),currentStat) == NULL)
        {
            qDebug() << "on sort du terrain";
            return NULL;
        }

        //pour cette sous-pièce c'est bon, on passe à la suivante
        iterator = iterator->next;
    }

    return NULL;
}

