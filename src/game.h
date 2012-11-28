#ifndef GAME_H
#define GAME_H

#include <QDomDocument>

#include "graph.h"
#include "list.h"
#include "tools.h"
#include "collection.h"
#include "matrix.h"

typedef Vector<Graph::Node*> State;

/*! \brief Gère le contenu du jeu : le plateau, les pièces et les règles.
 */
class Game
{
public:
    Game(const Game &original);
    Game& operator=(const Game& original);

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

    /*!
     *  \return L'état initial du jeu.
     */
    inline const Vector<Graph::Node*>& getInitialState() const {return m_initialState;}

    /*!
     *  \return L'état final du jeu.
     */
    inline const Vector<Graph::Node*>& getFinalState() const {return m_finalState;}

    /*!
     *  \return La liste des pièces du jeu
     */
    inline const List::Node<Graph::Node*>* getPieces() const {return m_pieces;}

    /*!
     *  \return Le nombre de pièces du jeu.
     */
    inline unsigned int getNbPieces() const {return List::size(m_pieces);}

    virtual ~Game();

private:
    Graph::Node *m_board; ///< Représente le tableau de jeu. Utile pour l'accès récursif.
    Graph::Node *m_jocker; ///< Représente la pièce jocker pour l'état final.
    Matrix<Graph::Node*> m_boardMatrix; ///< Représente le tableau de jeu. Utile pour l'accès en ligne/colonne.
    List::Node<Graph::Node*> *m_pieces; ///< Représente les pièces du jeu.
    Vector<Graph::Node*> m_index; ///< Permet de faire la liaison entre un état et les pièces.
    Vector<Graph::Node*> m_initialState; ///< Représente l'état initial du jeu.
    Vector<Graph::Node*> m_finalState; ///< Représente l'état final du jeu.
    unsigned int m_nbNodes; ///< Représente le nombre de cases existantes sur le plateau.
};

#endif // GAME_H
