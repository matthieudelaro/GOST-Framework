#include <QDebug>
#include <QMessageBox>
#include "IA.h"

namespace Gost
{
State* IA::possibleMove(const State& currentState, const Graph::Node* initialBoardNode, const Graph::Node* finalBoardNode, const GeneralGame &game)
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

List::Node<const State *>* IA::getPossibleMove(const State& currentState, const Graph::Node* piece, const GeneralGame &game)
{
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

List::Node<const State *>* IA::getPossibleMove(const State& currentState, const GeneralGame &game)
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

List::Node<const State *>* IA::aStar(const State &initialState,const State &finalState, const GeneralGame &game)
{
    AStarNode<const State*, unsigned int, unsigned int> *currentState = new AStarNode<const State*, unsigned int, unsigned int>;
    currentState->first = new State(initialState);
    currentState->second = 0;
    currentState->third = 0;
    currentState->parent = NULL;

    //                        état          g              h
    List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *openedList = NULL;
    List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *closedList = NULL;

    List::push_front(currentState, closedList);

    while(!( *(currentState->first) == finalState ))//noeud courant != noeud de destination => Approved :)
    {
        qDebug() << "On n'est pas à l'état final.";

        List::Node<const State *> *neighbours = IA::getPossibleMove( *(currentState->first), game);
        List::Node<const State *> *itNeighbours = neighbours;
        while(itNeighbours)//pour tous les voisins du noeud courant => Approved :)
        {
            qDebug() << "On regarde un nouveau voisin : " << itNeighbours;

            //si un noeud voisin est déjà dans la liste fermée, on l'oublie
                bool foundInClosedList = false;
                List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *itClosedList = closedList;
                while(itClosedList && !foundInClosedList)//approved :)
                {
                    if( *(itClosedList->info->first) == *(itNeighbours->info) )
                            foundInClosedList = true;
                    else
                        itClosedList = itClosedList->next;
                }
                qDebug() << "On a trouvé le voisin dans la liste fermée = " << foundInClosedList << " (" << itClosedList << " )";

            if(!foundInClosedList)//si un noeud voisin est déjà dans la liste fermée, on l'oublie
            {
                //si un noeud voisin est déjà dans la liste ouverte, on met à jour la liste ouverte si le noeud dans la liste ouverte a une moins bonne qualité (et on n'oublie pas de mettre à jour son parent)

            }
            //si un noeud voisin est déjà dans la liste ouverte, on met à jour la liste ouverte si le noeud dans la liste ouverte a une moins bonne qualité (et on n'oublie pas de mettre à jour son parent)
            //sinon, on ajoute le noeud voisin dans la liste ouverte avec comme parent le noeud courant
            itNeighbours = itNeighbours->next;
        }
        break;
        //Si la liste ouverte est vide, il n'y a pas de solution, fin de l'algorithme

        //On cherche le meilleur noeud de toute la liste ouverte.

        //On le met dans la liste fermée et on le retire de la liste ouverte
        //noeud courant = noeud que l'on vient d'ajouter à la liste fermée
    }

    List::Node<const State*> *resolutionPath = NULL;
    return resolutionPath;


//On commence par le noeud de départ, c'est le noeud courant
//On regarde tous ses noeuds voisins
    //si un noeud voisin est un obstacle, on l'oublie
    //si un noeud voisin est déjà dans la liste fermée, on l'oublie
    //si un noeud voisin est déjà dans la liste ouverte, on met à jour la liste ouverte si le noeud dans la liste ouverte a une moins bonne qualité (et on n'oublie pas de mettre à jour son parent)
    //sinon, on ajoute le noeud voisin dans la liste ouverte avec comme parent le noeud courant
//On cherche le meilleur noeud de toute la liste ouverte. Si la liste ouverte est vide, il n'y a pas de solution, fin de l'algorithme
//On le met dans la liste fermée et on le retire de la liste ouverte
//On réitère avec ce noeud comme noeud courant jusqu'à ce que le noeud courant soit le noeud de destination.

}







/*List::Node<const State *>* IA::aStar(const State &initialState,const State &finalState, const GeneralGame &game)
{
    //                        état          g              h
    List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *openNode = NULL;
    List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *closeNode = NULL;


    AStarNode<const State*, unsigned int, unsigned int> *tmpInitialNode = new AStarNode<const State*, unsigned int, unsigned int>;

    if(tmpInitialNode == NULL) //si l'allocation a échoué (parce qu'il n'y a plus de RAM libre par exemple)
    {
        BadAllocation exception("Unable to allocate memory to create a new Node.");
        throw exception;
    }

    //le noeud de base a la valeur minimal pour qu'il conserve sa place de premier
    tmpInitialNode->first = &initialState;
    tmpInitialNode->second = 0;
    tmpInitialNode->third = 0;
    tmpInitialNode->parent = NULL;

    List::push_front(tmpInitialNode,closeNode);

    //On commence par le noeud de départ, c'est le noeud courant
    AStarNode<const State*, unsigned int, unsigned int> *currentState = closeNode->info;

    //while(IA::stateValue(currentState->first,game) != IA::stateValue(finalState,game)) // tant que l'on a pas l'état final
    while (!(*(currentState->first) == finalState))
    {
        qDebug() << "nouvel état";
        //on récupères tous les voisins de l'état courant
        List::Node<const State *> *neighbours = IA::getPossibleMove(*(currentState->first),game);

        while(neighbours) // on parcours tous les voisins
        {
            qDebug() << "nouveau voisin";
            AStarNode<const State*, unsigned int, unsigned int> *tmpNeighbour = new AStarNode<const State*, unsigned int, unsigned int>;

            if(tmpNeighbour == NULL) //si l'allocation a échoué (parce qu'il n'y a plus de RAM libre par exemple)
            {
                BadAllocation exception("Unable to allocate memory to create a new Node.");
                throw exception;
            }

            tmpNeighbour->first = neighbours->info;
            tmpNeighbour->second = currentState->second + 1;
            tmpNeighbour->third = 1;  //temporaire en attente du calcul de h
            tmpNeighbour->parent = currentState;

            if(!List::contains(tmpNeighbour,closeNode)) //s'il est dans la liste fermée alors on le laisse, il est bien placé
            {
                //on regarde si on ne passe pas sur un noeud déjà visité
                List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *findResultList = List::find(tmpNeighbour,openNode);
                qDebug() << "result : " << findResultList;
                tmpNeighbour->third = IA::hScore(*(neighbours->info),finalState,game);
                if(findResultList) //s'il est dans la liste ouverte on regarde s'il est plus avantageux
                {
                    AStarNode<const State*, unsigned int, unsigned int> *findResult = findResultList->info;

                    if(tmpNeighbour->second < findResult->second)
                    {//si le nouveau passage sur ce noeud est plus avantageux on modifie son homologue dans la liste ouverte
                        findResult->second = tmpNeighbour->second;
                        findResult->third = tmpNeighbour->third;
                        findResult->parent = tmpNeighbour;
                    }
                }
                else //sinon on le met dans la liste ouverte
                {
                    List::push_front(tmpNeighbour,openNode);
                    qDebug() << "ajout de l'état dans open node";
                }
            }
            neighbours = neighbours->next;
        }

        if(openNode) //s'il y a encore des noeuds ouverts
        {
            //recherche du noeud avec la plus petite valeur de f
            List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *tmpBestNodeFromOpenList = openNode;
            List::Node<AStarNode<const State*, unsigned int, unsigned int> *> *BestNodeFromOpenList = openNode;

            while(tmpBestNodeFromOpenList)
            {
                qDebug() << "nouveau noeud max";
                if((tmpBestNodeFromOpenList->info->second +
                    tmpBestNodeFromOpenList->info->third) <
                        (BestNodeFromOpenList->info->second +
                         BestNodeFromOpenList->info->third))
                    BestNodeFromOpenList = tmpBestNodeFromOpenList;
                tmpBestNodeFromOpenList = tmpBestNodeFromOpenList->next;
            }

            AStarNode<const State*, unsigned int, unsigned int> *bestNodeToAddInCloseNode = BestNodeFromOpenList->info;
            List::remove(BestNodeFromOpenList,openNode);
            List::push_front(bestNodeToAddInCloseNode,closeNode);
            currentState = bestNodeToAddInCloseNode;
        }
        else //sinon il n'y a pas de solutions
        {
            qDebug() << "pas de solutions";
            return NULL;
        }
    }

    List::Node<const State *>* toReturn = NULL;

//    while(closeNode)
//    {
//        List::push_front(closeNode->info->first,toReturn);
//        closeNode = closeNode->next;
//    }

    return toReturn;
}*/




unsigned int IA::gScore(const State& currentState, const State& initialState, const State &finalState, const GeneralGame &game)
{
    //improve this function ?
    return 0;
}


unsigned int IA::hScore(const State& currentState, const State &finalState, const GeneralGame &game)
{
    //principe : on regarde combien de noeuds sont à une position différente de la fin
    unsigned int hScore = 0;
    for(unsigned int i = 0; i < game.getNbNodes() ; i ++)
    {
        const Graph::Node* finalNode = game.getPieceNode(i,finalState);
        const Graph::Node* currentNode = game.getPieceNode(i,currentState);
        if(finalNode) //si on a une case à tester
        {
            if(finalNode->info != 0) //si on ne teste pas un joker
            {
                if(!currentNode)//si la case courante est vide
                    hScore ++;
                else if(currentNode->info != finalNode->info)//si les deux cases ne contiennent pas la même pièce
                    hScore ++;
            }
        }
        else if (currentNode) //si la case est vide à l'état final (car else if), mais non vide à l'état courant
            hScore ++;
    }
    return hScore;
}
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
