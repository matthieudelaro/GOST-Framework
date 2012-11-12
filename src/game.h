#ifndef GAME_H
#define GAME_H

#include <QDomDocument>

#include "graph.h"
#include "list.h"
#include "tools.h"
#include "collection.h"
#include "matrix.h"

class Game
{
public:
    Game();
    bool load(QDomDocument &xml);
    inline unsigned int getNbNodes() const {return m_nbNodes;}
    const Graph::Node*& getNodePiece(unsigned int index, const Matrix<Graph::Node*> &etat) const;
    unsigned int getNumberPiece(unsigned int index, const Matrix<Graph::Node*> &etat) const;
    inline Matrix<Graph::Node*>* getBoardMatrix() {return &m_boardMatrix;}
    ~Game();

private:
    Graph::Node *m_board;
    Matrix<Graph::Node*> m_boardMatrix;
    List::Node<Graph::Node*> *m_pieces;
    Graph::Node* *m_index; //tableau de pointeurs de Graph::Node
    unsigned int m_nbNodes;
};

#endif // GAME_H
