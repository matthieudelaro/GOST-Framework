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
    //Game(const Game &original); Il faut absolument instancier ce constructeur de copie
    Game();
    bool load(QDomDocument &xml);
    inline unsigned int getNbNodes() const {return m_nbNodes;}
    const Graph::Node*& getNodePiece(unsigned int index, const Vector<Graph::Node*> &etat) const;//finalement je crois que cette fonction n'a rien à faire ici
    unsigned int getNumberPiece(unsigned int index, const Vector<Graph::Node*> &etat) const;//finalement je crois que cette fonction n'a rien à faire ici
    inline Matrix<Graph::Node*>* getBoardMatrix() {return &m_boardMatrix;}
    ~Game();

private:
    Graph::Node *m_board;
    Matrix<Graph::Node*> m_boardMatrix;
    List::Node<Graph::Node*> *m_pieces;
    //Graph::Node* *m_index; //tableau de pointeurs de Graph::Node
    Vector<Graph::Node*> m_index;
    unsigned int m_nbNodes;
};

#endif // GAME_H
