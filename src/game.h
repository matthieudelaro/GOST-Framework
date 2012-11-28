#ifndef GAME_H
#define GAME_H

#include <QDomDocument>

#include "graph.h"
#include "list.h"
#include "tools.h"
#include "collection.h"
#include "matrix.h"

typedef Vector<Graph::Node*> State;

class Game
{
public:
    //Game(const Game &original); Il faut absolument instancier ce constructeur de copie
    Game();
    bool load(QDomDocument &xml);
    inline unsigned int getNbNodes() const {return m_nbNodes;}
    //const Graph::Node*& getNodePiece(unsigned int index, const Vector<Graph::Node*> &etat) const;//finalement je crois que cette fonction n'a rien à faire ici
    //unsigned int getNumberPiece(unsigned int index, const Vector<Graph::Node*> &etat) const;//finalement je crois que cette fonction n'a rien à faire ici
    inline Matrix<Graph::Node*>* getBoardMatrix() {return &m_boardMatrix;}

    /*! \brief Effectue la liaison entre un index et un noeud du plateau.
     *
     *  \param index L'index dont on veut obtenir le noeud correspondant.
     *  \return Un pointeur vers le noeud correspondant.
     */
    inline const Graph::Node* getBoardNode(unsigned int index) const { return m_index[index];}

    /*! \brief Effectue la liaison entre un noeud du plateau, et le noeud de la pièce qui l'occupe.
     *
     *  \param boardNode Le noeud du plateau.
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud de la pièce.
     *  \return Un pointeur vers le noeud de la pièce qui occupe boardNode.
     */
    inline const Graph::Node* getPieceNode(const Graph::Node* &boardNode, const Vector<Graph::Node*> &state) const { return state[boardNode->info];}

    /*! \brief Effectue la liaison entre une position sur le plateau, et le noeud de la pièce qui l'occupe.
     *
     *  \param line Ordonnée de la position.
     *  \param column Abscisse de la position.
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud de la pièce.
     *  \return Un pointeur vers le noeud de la pièce qui occupe boardNode.
     */
    const Graph::Node* getPieceNode(unsigned int line, unsigned int column, const Vector<Graph::Node*> &state) const
    {
        if(m_boardMatrix(line,column) == NULL)
            return NULL;
        return state[m_boardMatrix(line, column)->info];
    }

    /*
    /*! \brief Effectue la liaison entre un index, et le noeud de la pièce qui l'occupe.
     *
     *  \param index L'index dont on veut obtenir la pièce
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud de la pièce.
     *  \return Un pointeur vers le noeud de la pièce correspondant à l'index.

    inline const Graph::Node* getPieceNode(unsigned int index, const Vector<Graph::Node*> &state) const { return state[index];}
    */

    /*!
     *  \return L'état initial du jeu.
     */
    inline const Vector<Graph::Node*>& getInitialState() const {return m_initialState;}

    /*!
     *  \return L'état final du jeu.
     */
    inline const Vector<Graph::Node*>& getFinalState() const {return m_finalState;}

    ~Game();

private:
    Graph::Node *m_board;
    Matrix<Graph::Node*> m_boardMatrix;
    List::Node<Graph::Node*> *m_pieces;
    Vector<Graph::Node*> m_index;
    Vector<Graph::Node*> m_initialState;
    Vector<Graph::Node*> m_finalState;
    unsigned int m_nbNodes;
};

#endif // GAME_H
