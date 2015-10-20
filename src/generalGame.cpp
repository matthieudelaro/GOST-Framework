#include "generalGame.h"
namespace Gost
{
bool GeneralGame::getNodePieceIndex(const Graph::Node* &pieceNode, unsigned int &index, const State &state) const
{
    for(index = 0; index < state.getLength(); ++index)
        if(state[index] == pieceNode)
            return true;

    //si on n'a pas trouvé la pièce
    return false;
}

GeneralGame::GeneralGame()
{
    m_board = NULL;
    m_jocker = NULL;
    m_pieces = NULL;
    m_nbNodes = 0;
}

void GeneralGame::clear()
{
    //Supprimer le m_board
    Graph::clear(m_board);

    //Supprimer les pièces
    Graph::clear(m_jocker);
    List::Node<Graph::Node*>* it = m_pieces;
    while(it)
    {
        delete it->info;
        it = it->next;
    }
    List::clear(m_pieces);
}

const Graph::Node *GeneralGame::getBoardNode(const Graph::Node *&pieceNode, const State &state) const
{
    for(unsigned int i = 0; i < state.getLength(); ++i)
           if(state[i] == pieceNode)
               return m_index[i];

       //si on n'a pas trouvé
       return NULL;
}
}
