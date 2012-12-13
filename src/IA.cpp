#include <QDebug>
#include <QMessageBox>
#include "IA.h"

State* IA::possibleMove(const State& currentState, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const Game &game)
{
    //qDebug() << "######################################################";
    //qDebug() << "start" << initialBoardNode << "end " << finalBoardNode;
    //on vérifie que les 2 noeuds existent
    if(initialBoardNode == NULL || finalBoardNode == NULL)
    {
        return NULL ;
    }

    //qDebug() << "les deux cases existent";

    //vérification que les deux noeuds sont ajacents
    //on regarde si les deux liens sont bien voisins et on récupère la direction
    int direction = -1;
    for(unsigned int i = 0; i < initialBoardNode->nbLinks; i ++)
    {
        //qDebug() << i << initialBoardNode->getConstLink(i);
        if(initialBoardNode->getConstLink(i) == finalBoardNode)
            direction = i;
    }
    if(direction == -1)//si ce ne sont pas des voisins
    {
        return NULL;
    }
    //qDebug() << "les deux cases sont voisines";

    //on vérifie que l'on ne prend pas une case vide (qu'on l'on n'essaye pas de déplacer une case vide)
    if(game.getPieceNode(initialBoardNode, currentState) == NULL)
    {
        return NULL;
    }
    //qDebug() << "on ne prend pas une piece vide";

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
            //qDebug() << "Impossible de retrouver la pièce !";
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
                //qDebug() << "La destination d'un des noeuds de la pièce est déjà occupée.";
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
            //qDebug() << "La destination d'un des noeuds de la pièce n'existe pas";
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
    ////qDebug() << "debut test fin";
    for(unsigned int line = 0; line < game->getBoardMatrix()->getHeight() ; ++line)
    {
        for(unsigned int column = 0; column < game->getBoardMatrix()->getWidth() ; ++column)
        {
            ////qDebug() << line << column;
            if(game->getPieceNode(line,column,endState))
            {
                ////qDebug() << "il existe une pièce à l'arrivée";
                if(game->getPieceNode(line,column,endState)->info == 0)
                {
                    ////qDebug() << "on a un jocker : on passe";
                }
                else if(!game->getPieceNode(line,column,currentState))
                {
                    ////qDebug() << "on n'a pas de pièce maintenant";
                    return false;
                }
                else
                {//si la case d'arrivée n'est pas un jocker on vérifie les compatibilités
                    ////qDebug() << game->getPieceNode(line,column,endState)->info << "? = " << game->getPieceNode(line,column,currentState)->info;
                    if(game->getPieceNode(line,column,endState)->info != game->getPieceNode(line,column,currentState)->info)
                        return false;
                }
            }
            if(!game->getPieceNode(line,column,endState) && game->getPieceNode(line,column,currentState))
            {
                ////qDebug() << "Il y a une pièce alors qu'il ne doit pas y en avoir à la fin.";
                return false;
            }
        }
    }
    return true;
}

List::Node<const State *>* IA::getPossibleMove(const State& currentState, const Graph::Node* piece, const Game &game)
{
    //qDebug() << "getpossible move on piece : " << game.getBoardNode(piece,currentState) << piece->info;

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

List::Node<const State *>* IA::aStar(const State &initialState,const State &finalState, const Game &game)
{
    /*VERSION 1 : DEVELOPPEZ*/

    //                        état          g              h          parent
    List::Node<Quadruple<const State*, unsigned int, unsigned int, const State *> *> *openNode;
    List::Node<Quadruple<const State*, unsigned int, unsigned int, const State *> *> *closeNode;


    Quadruple<const State*, unsigned int, unsigned int, const State *> *tmpInitialNode = new Quadruple<const State*, unsigned int, unsigned int, const State *>;
    //le noeud de base a la valeur minimal pour qu'il conserve sa place de premier
    tmpInitialNode->first = &initialState;
    tmpInitialNode->second = 0;
    tmpInitialNode->third = 0;

    List::push_front(tmpInitialNode,closeNode);
    //    On commence par le noeud de départ, c'est le noeud courant
    Quadruple<const State*, unsigned int, unsigned int, const State *> *currentState = closeNode->info;

    //while(IA::stateValue(currentState->first,game) != IA::stateValue(finalState,game)) // tant que l'on a pas l'état final
    while (!((*(currentState->first)) == finalState))
    {

        //on récupères tous les voisins de l'état courant
        List::Node<const State *> *neighbours = IA::getPossibleMove(*(currentState->first),game);

        while(neighbours) // on parcours tous les voisins
        {
            Quadruple<const State*, unsigned int, unsigned int, const State *> *tmpNeighbour = new Quadruple<const State*, unsigned int, unsigned int, const State *>;
            tmpNeighbour->first = neighbours->info;
            tmpNeighbour->second = currentState->second + 1;
            tmpNeighbour->third = 1;  //temporaire en attente du calcul de h
            tmpNeighbour->fourth = currentState->first;

            if(!List::contains(tmpNeighbour,closeNode)); //s'il est dans la liste fermée alors on le laisse, il est bien placé
            {
                //on regarde si on ne passe pas sur un noeud déjà visité
                List::Node<Quadruple<const State*, unsigned int, unsigned int, const State *> *> *findResultList = List::find(tmpNeighbour,openNode);

                if(findResultList) //s'il est dans la liste ouverte on regarde s'il est plus avantageux
                {
                    Quadruple<const State*, unsigned int, unsigned int, const State *> *findResult = findResultList->info;
                    tmpNeighbour->third = IA::hScore(*(neighbours->info),finalState,game);

                    if((tmpNeighbour->second + tmpNeighbour->third) < (findResult->second + findResult->third))
                    {//si le nouveau passage sur ce noeud est plus avantageux on modifie son homologue dans la liste ouverte
                        findResult->second = tmpNeighbour->second;
                        findResult->third = tmpNeighbour->third;
                        findResult->fourth = tmpNeighbour->fourth;
                    }
                }
                else //sinon on le met dans la liste ouverte
                {
                    List::push_front(tmpNeighbour,openNode);
                }
            }
        }

        if(openNode) //s'il y a encore des noeuds ouverts
        {
            //recherche du noeud avec la plus petite valeur de f
            List::Node<Quadruple<const State*, unsigned int, unsigned int, const State *> *> *tmpBestNodeFromOpenList = openNode;
            List::Node<Quadruple<const State*, unsigned int, unsigned int, const State *> *> *BestNodeFromOpenList = openNode;

            while(tmpBestNodeFromOpenList)
            {
                if((tmpBestNodeFromOpenList->info->second +
                    tmpBestNodeFromOpenList->info->third) < (BestNodeFromOpenList->info->second + BestNodeFromOpenList->info->third))
                BestNodeFromOpenList = tmpBestNodeFromOpenList;
                tmpBestNodeFromOpenList = tmpBestNodeFromOpenList->next;
            }

            Quadruple<const State*, unsigned int, unsigned int, const State *> *bestToAddInCloseNode = BestNodeFromOpenList->info;
            List::remove(BestNodeFromOpenList,openNode);
            List::push_front(bestToAddInCloseNode,closeNode);
            currentState = bestToAddInCloseNode;
        }
        else //sinon il n'y a pas de solutions
        {
            qDebug( ) << "pas de solutions";
            return NULL;
        }
    }

    List::Node<const State *>* toReturn;

    while(closeNode)
    {
        List::push_front(closeNode->info->first,toReturn);
        closeNode = closeNode->next;
    }

    return toReturn;
}




unsigned int IA::gScore(const State& currentState, const State& initialState, const State &finalState, const Game &game)
{
    //improve this function ?
}


unsigned int IA::hScore(const State& currentState, const State &finalState, const Game &game)
{
    //principe : on regarde combien de noeuds sont à une position différente de la fin

    unsigned int hScore = 0;
    for(unsigned int i = 0; i < game.getNbNodes() ; i ++)
    {
        const Graph::Node* finalNode = game.getPieceNode(i,finalState);
        if(finalNode) //si on a une case à tester
        {
            if(finalNode->info != 0) //si on ne teste pas un joker
            {
                const Graph::Node* currentNode = game.getPieceNode(i,currentState);
                if(currentNode->info != finalNode->info) //si les deux pièces sont différentes on incrémente le score
                    hScore ++;
            }
        }
        else if (game.getPieceNode(i,currentState)) //si la case est vide à l'état final (car else if), mais non vide à l'état courant
            hScore++;
    }

    return hScore;
}


/*VERSION 2 wikipedia

List::Node<const State *> *openNode;
List::Node<const State *> *closeNode;
List::push_front(initialState,closeNode);

// ??? came_from := the empty map    // The map of navigated nodes.

while(List::size(openNode) != 0)
{

    List::Node<const State *> * current = List::worst(openNode);
    if(current = finalState)
        return ;//chemin

    List::remove(current,openNode);
    List::push_front(current,closeNode);

    List::Node<const State *> * neighbourg = IA::getPossibleMove(current->info,game);
    while(neighbourg)
    {
        if(List::contains(neighboug,closeNode))
            neighbourg = neighbourg->next;
        else
        {
            unsigned int gScoreCurrent = IA::gScore(current,initialState,FinalState,game)  + IA::heuristicScore(current,initialState,FinalState,game); //tentative_g_score := g_score[current] + dist_between(current,neighbor)
            unsigned int gScoreNeighbourg = IA::gScore(neighbourg,initialState,FinalState,game)  + IA::heuristicScore(neighbourg,initialState,FinalState,game); //tentative_g_score := g_score[current] + dist_between(current,neighbor)


            if(!List::contains(neighbourg) || (gScoreCurrent <= gScoreNeighbourg ))
            {



            }


                ???? came_from[neighbor] := current
                g_score[neighbor] := tentative_g_score
                f_score[neighbor] := g_score[neighbor] + heuristic_cost_estimate(neighbor, goal)
                if neighbor not in openset
                    add neighbor to openset


        }

    }

}
*/

////pour différencier deux états
//unsigned int IA::stateValue(const State& state, const Game &game)
//{
//    List::Node<Graph::Node*>* piece = game.getPieces();
//    unsigned int value = 0;
//    while(piece)
//    {
//        value +=(unsigned int)(game.getBoardNode(piece->info,state)); //cast pour obtenir une valeur moche
//        piece = piece->next;
//    }
//    return value;
//}
