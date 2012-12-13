#ifndef GENERALGAME_H
#define GENERALGAME_H

#include "graph.h"
#include "list.h"
#include "tools.h"
#include "collection.h"
#include "matrix.h"

/*!
 *  Le namespace Gost contient tous les éléments du framework Gost.
 */
namespace Gost
{
typedef Vector<const Graph::Node*> State;///< Représente un état du jeu. Chaque case correspond à une case existante du plateau, et contient un pointeur vers le morceau de pièce qui la chevauche.

/*!
 * \brief La classe GeneralGame est une classe abstraite qui permet de définir un jeu.
 *
 * GeneralGame définit un jeu générique. En héritant de cette classe, il est possible de faire
 * n'importe quel type de jeu avec des pièces coulissantes, en 2D, 3D, de formes diverses et variées,
 * avec un plateau cyclique (les pièces sortant d'un côté rentreraient du côté opposé), ...
 *
 * La seule limite est l'imagination du programmeur qui l'utilisera.
 */
class GeneralGame
{
public :
    virtual ~GeneralGame() {clear();}///< Le destructeur.

    /*! \brief Vide le jeu.
     */
    virtual void clear();

    /*! \return Le nombre de cases existantes sur le plateau. Tout State doit avoir cette longueur.
     */
    inline unsigned int getNbNodes() const {return m_nbNodes;}

    /*! \brief Effectue la liaison entre un index et un noeud du plateau.
     *
     *  \param index L'index dont on veut obtenir le noeud correspondant.
     *  \return Un pointeur vers le noeud correspondant.
     */
    inline const Graph::Node* getBoardNode(unsigned int index) const { return m_index[index];}

    /*! \brief Effectue la liaison entre un noeud d'une pièce et un noeud du plateau.
     *
     *  \param pieceNode Le noeud de la pièce.
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud du plateau.
     *  \return Un pointeur vers le noeud correspondant.
     */
    const Graph::Node* getBoardNode(const Graph::Node* &pieceNode, const State &state) const;

    /*! \brief Effectue la liaison entre un noeud d'une pièce et l'index correspondant.
     *
     *  \param pieceNode Le noeud de la pièce.
     *  \param index Référence dans laquelle sera stocké l'index en cas de succès. En cas d'échec, la valeur d'index est indéfinie.
     *  \param state L'état du jeu auquel on souhaite obtenir l'index.
     *  \return Return true si on a trouvé la pièce dans l'index, et false si on ne l'a pas trouvé.
     */
    bool getNodePieceIndex(const Graph::Node* &pieceNode, unsigned int &index, const State &state) const;

    /*! \brief Effectue la liaison entre un noeud du plateau, et le noeud de la pièce qui l'occupe.
     *
     *  \param boardNode Le noeud du plateau.
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud de la pièce.
     *  \return Un pointeur vers le noeud de la pièce qui occupe boardNode.
     */
    inline const Graph::Node* getPieceNode(const Graph::Node* &boardNode, const State &state) const { return state[boardNode->info];}

    /*! \brief Effectue la liaison entre un index, et le noeud de la pièce qui l'occupe.
     *
     *  \param index L'index pour lequel on recherche la pièce.
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud de la pièce.
     *  \return Un pointeur vers le noeud de la pièce.
     */
    inline const Graph::Node* getPieceNode(unsigned int index, const State &state) const { return state[index];}

    /*!
     *  \return L'état initial du jeu.
     */
    inline const State& getInitialState() const {return m_initialState;}

    /*!
     *  \return L'état initial du jeu.
     */
    inline State* getInitialStateCopy() const {return new State(m_initialState);}

    /*!
     *  \return L'état final du jeu.
     */
    inline const State& getFinalState() const {return m_finalState;}

    /*!
     *  \return La liste des pièces du jeu
     */
    inline const List::Node<Graph::Node*>* getPieces() const {return m_pieces;}

    /*!
     *  \return Le nombre de pièces du jeu.
     */
    virtual inline unsigned int getNbPieces() const {return List::size(m_pieces);}

protected :
    GeneralGame();///< Le constructeur.

    /*!
     * \brief Le constructeur de copie.
     *
     *  Le constructeur de copie ne doit jamais être appelé, même par la classe GeneralGame elle-même,
     *  car il entraînerait une contre-performance. Le déclarer comme private ne suffisant pas,
     *  il génère aussi une exception lors de l'exécution.
     *
     * \param original Le jeu copié.
     */
    GeneralGame(const GeneralGame &original);

    /*!
     * \brief L'opérateur d'affectation.
     *
     *  L'opérateur d'affectation ne doit jamais être appelé, même par la classe GeneralGame elle-même,
     *  car il entraînerait une contre-performance. Le déclarer comme private ne suffisant pas,
     *  il génère aussi une exception lors de l'exécution.
     *
     * \param original Le jeu copié.
     */
    GeneralGame& operator=(const GeneralGame& original);

    Graph::Node *m_board; ///< Représente le tableau de jeu. Utile pour l'accès récursif.
    Graph::Node *m_jocker; ///< Représente la pièce jocker pour l'état final.
    List::Node<Graph::Node*> *m_pieces; ///< Représente les pièces du jeu.
    Vector<Graph::Node*> m_index; ///< Permet de faire la liaison entre un état et les pièces.
    State m_initialState; ///< Représente l'état initial du jeu.
    State m_finalState; ///< Représente l'état final du jeu. Attention : ce n'est pas un état valide. Il ne faut tenir compte que du numéro des pièces, car il peut arriver qu'un noeud de pièce soit à plusieurs endroits à la fois !
    unsigned int m_nbNodes; ///< Représente le nombre de cases existantes sur le plateau.
};
}

#endif // GENERALGAME_H
