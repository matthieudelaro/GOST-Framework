#include <QDebug>
#include "IA.h"

State* IA::possibleMove(const State& currentState, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const Game &game)
{
    qDebug() << "######################################################";
    qDebug() << "start" << initialBoardNode << "end " << finalBoardNode;
    //on vérifie que les 2 noeuds existent
    if(initialBoardNode == NULL || finalBoardNode == NULL)
    {
        return NULL ;
    }

    qDebug() << "les deux cases existent";

    //vérification que les deux noeuds sont ajacents
    //on regarde si les deux liens sont bien voisins et on récupère la direction
    int direction = -1;
    for(unsigned int i = 0; i < initialBoardNode->nbLinks; i ++)
    {
        qDebug() << i << initialBoardNode->getConstLink(i);
        if(initialBoardNode->getConstLink(i) == finalBoardNode)
            direction = i;
    }
    if(direction == -1)//si ce ne sont pas des voisins
    {
        return NULL;
    }
    qDebug() << "les deux cases sont voisines";

    //on vérifie que l'on ne prend pas une case vide (qu'on l'on n'essaye pas de déplacer une case vide)
    if(game.getPieceNode(initialBoardNode, currentState) == NULL)
    {
        return NULL;
    }
    qDebug() << "on ne prend pas une piece vide";

    //on parcoure tous les noeuds de la pièce pour vérifier que chaque noeud peut être déplacé
    //on en profite pour prendre en note les déplacements à effectuer si déplacement est autorisé
    List::Node<Triple<unsigned int, unsigned int, const Graph::Node*>* > *moves = NULL;//l'index de la case source, l'index de la case destination, et le Node de la pièce qu'il faut mettre dedans
    //algorithme actuel :
    //  - on récupère tous les noeuds de la pièce
    //  - on fait un test pour chacun d'eux -> long car il faut parcourir tout l'état pour retrouver le noeud du plateau correspondant
    //meilleur algorithme (mais casse-tête à écrire) :
    //  - parcourir en même temps le graph de la pièce et le graph du plateau
    List::Node<const Graph::Node*>* nodesToMove = Graph::toConstList(game.getPieceNode(initialBoardNode, currentState));
    List::Node<const Graph::Node*>* it = nodesToMove;
    while(it)
    {
        //if(it->info->getConstLink(direction))
        unsigned int indexSource;
        if(!game.getNodePieceIndex(it->info, indexSource, currentState))
        {
            qDebug() << "Impossible de retrouver la pièce !";
            return NULL;
        }
        const Graph::Node* destinationNode = game.getBoardNode(indexSource)->getConstLink(direction);
        //if(Game.getPieceNode(destinationNode, currentStat) == NULL//si la case est libre
        //        || Game.getPieceNode(destinationNode, currentStat)->info == game.getPieceNode(initialBoardNode, currentStat)->info)//on si elle est occupée par la même pièce (qui va donc bouger)
            //alors on peut déplacer
        if(destinationNode)
        {
            if(game.getPieceNode(destinationNode, currentState) != NULL && game.getPieceNode(destinationNode, currentState)->info != game.getPieceNode(initialBoardNode, currentState)->info)
            {
                qDebug() << "La destination d'un des noeuds de la pièce est déjà occupée.";
                return NULL;
            }
            else
            {
                Triple<unsigned int, unsigned int, const Graph::Node*> *move = new Triple<unsigned int, unsigned int, const Graph::Node*>;
                move->first = indexSource;//l'index de la source du mouvement
                move->second = destinationNode->info;////l'index de la destination du mouvement
                move->third = it->info;//le Node de pièce à y placer
                List::push_front(move, moves);
            }
        }
        else
        {
            qDebug() << "La destination d'un des noeuds de la pièce n'existe pas";
            return NULL;
        }
        it = it->next;
    }

    //et puisqu'on peut, on déplace
    State *afterMove = new State(currentState);
    List::Node<Triple<unsigned int, unsigned int, const Graph::Node*>* > *itMoves = moves;
    //on supprime les noeuds de la pièce qu'on déplace
    while(itMoves)
    {
        afterMove->get(itMoves->info->first) = NULL;
        itMoves = itMoves->next;
    }

    //puis on place correctement les noeuds de la pièce qu'on déplace
    itMoves = moves;
    while(itMoves)
    {
        afterMove->get(itMoves->info->second) = itMoves->info->third;
        itMoves = itMoves->next;
    }

    List::clearDelete(moves);
    List::clear(nodesToMove);
    return afterMove;
}

bool IA::isEnd(const State& currentState, const State& endState, Game *game)
{
    //qDebug() << "debut test fin";
    for(unsigned int line = 0; line < game->getBoardMatrix()->getHeight() ; ++line)
    {
        for(unsigned int column = 0; column < game->getBoardMatrix()->getWidth() ; ++column)
        {
            //qDebug() << line << column;
            if(game->getPieceNode(line,column,endState))
            {
                //qDebug() << "il existe une pièce à l'arrivée";
                if(game->getPieceNode(line,column,endState)->info ==  0)
                {
                    //qDebug() << "on a un jocker : on passe";
                }
                else if(!game->getPieceNode(line,column,currentState))
                {
                    //qDebug() << "on n'a pas de pièce maintenant";
                    return false;
                }
                else
                {//si la case d'arrivée n'est pas un jocker on vérifie les compatibilités
                    //qDebug() << game->getPieceNode(line,column,endState)->info << "? = " << game->getPieceNode(line,column,currentState)->info;
                    if(game->getPieceNode(line,column,endState)->info != game->getPieceNode(line,column,currentState)->info)
                        return false;
                }
            }
        }
    }
    return true;
}

List::Node<const State *>* IA::getPossibleMove(const State& currentState, const Graph::Node* piece, const Game &game)
{
    qDebug() << "getpossible move on piece : " << game.getBoardNode(piece,currentState) << piece->info;

    List::Node<const State *>* possibleMoves = NULL;
    for(unsigned int i = 0; i < Graph::Node::nbLinks ; i ++)
    {
        //pour tous les suivants possibles de piece
        const Graph::Node* boardNodeToTest = game.getBoardNode(piece,currentState);
        //si on a bien un noeud

        const State* possibleState = IA::possibleMove(currentState, boardNodeToTest, boardNodeToTest->getConstLink(i), game);
        if(possibleState)
            List::push_front(possibleState, possibleMoves);

    }
    return possibleMoves;
}

List::Node<const State *>* IA::getPossibleMove(const State& currentState, const Game &game)
{
    const List::Node<Graph::Node *>* piecesToCheck = game.getPieces();

    List::Node<const State *>* possibleMoves  = NULL;

    while(piecesToCheck)
    {

        List::Node<const State *> *moveToTest = IA::getPossibleMove(currentState,piecesToCheck->info,game);

        if(moveToTest)
            List::push_front(possibleMoves, moveToTest);

        piecesToCheck = piecesToCheck->next;
    }
    return possibleMoves;
}

