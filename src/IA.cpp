#include <QDebug>
#include "IA.h"

const Vector<Graph::Node*>* IA::possibleMove(const State& currentStat, const Graph::Node* initPieceNode, const Graph::Node* finalPieceNode, const Game &game)
{
    qDebug() << "nouveau deplacement";

    if(initPieceNode != NULL)
        qDebug() << initPieceNode->info;
    if(finalPieceNode != NULL)
        qDebug() << finalPieceNode->info;



    return NULL;
}

