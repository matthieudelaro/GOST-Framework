#ifndef GAME_H
#define GAME_H

#include <QDomDocument>

#include "graph.h"
#include "list.h"
#include "tools.h"
#include "collection.h"
#include "matrix.h"

typedef Vector<const Graph::Node*> State;///< Représente un état du jeu. Chaque case correspond à une case existante du plateau, et contient un pointeur vers le morceau de pièce qui la chevauche.

/*! \brief Gère le contenu du jeu : le plateau, les pièces et les règles.
 */
class Game
{
public:

    Game();

    /*!
     * \brief Permet de charger un jeu au format xml.
     * \param xml Le fichier à charger.
     * \param error Si error est différent de NULL, alors error est utilisé pour renvoyer les erreurs de lecture du fichier xml
     * \return true si le chargement a été effectué avec succès, false sinon.
     */
    bool load(QDomDocument &xml, QString *error = NULL);

    /*!
     * \brief Permet de charger un jeu au format Pepper
     *
     *  Cette fonction génère un xml à partir de file, et appel Game::load(QDomDocument &xml, QString *error = NULL) avec.
     *
     * \param file Le fichier à charger.
     * \param error Si error est différent de NULL, alors error est utilisé pour renvoyer les erreurs de lecture du fichier xml
     * \return true si le chargement a été effectué avec succès, false sinon.
     */
    bool load(const QString &file, QString *error = NULL);

    /*! \return Le nombre de cases existantes sur le plateau. Tout State doit avoir cette longueur.
     */
    inline unsigned int getNbNodes() const {return m_nbNodes;}

    /*! \return La Matrix du plateau.
     */
    inline Matrix<Graph::Node*>* getBoardMatrix() {return &m_boardMatrix;}

    /*! \brief Effectue la liaison entre un index et un noeud du plateau.
     *
     *  \param index L'index dont on veut obtenir le noeud correspondant.
     *  \return Un pointeur vers le noeud correspondant.
     */
    inline const Graph::Node* getBoardNode(unsigned int index) const { return m_index[index];}

    /*! \brief Effectue la liaison entre des coordonnées 2D et un noeud du plateau.
     *
     *  \param line La ligne dont on veut obtenir le noeud correspondant.
     *  \param column La colonne dont on veut obtenir le noeud correspondant.
     *  \return Un pointeur vers le noeud correspondant.
     */
    inline const Graph::Node* getBoardNode(unsigned int line, unsigned int column) const { return m_boardMatrix.getConst(line, column);}

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

    /*! \brief Effectue la liaison entre une position sur le plateau, et le noeud de la pièce qui l'occupe.
     *
     *  \param line Ordonnée de la position.
     *  \param column Abscisse de la position.
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud de la pièce.
     *  \return Un pointeur vers le noeud de la pièce qui occupe boardNode.
     */
    const Graph::Node* getPieceNode(unsigned int line, unsigned int column, const State &state) const;

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
    inline unsigned int getNbPieces() const {return List::size(m_pieces);}

    /*! \brief Vide le jeu.
     */
    void clear();

    virtual ~Game();

private:
    /*!
     * \brief Le constructeur de copie.
     *
     *  Le constructeur de copie ne doit jamais être appelé, même par la classe Game elle-même,
     *  car il entraînerait une contre-performance. Le déclarer comme private ne suffisant pas,
     *  il génère aussi une exception lors de l'exécution.
     *
     * \param original Le jeu copié.
     */
    Game(const Game &original);

    /*!
     * \brief L'opérateur d'affectation.
     *
     *  L'opérateur d'affectation ne doit jamais être appelé, même par la classe Game elle-même,
     *  car il entraînerait une contre-performance. Le déclarer comme private ne suffisant pas,
     *  il génère aussi une exception lors de l'exécution.
     *
     * \param original Le jeu copié.
     */
    Game& operator=(const Game& original);

protected :
    Graph::Node *m_board; ///< Représente le tableau de jeu. Utile pour l'accès récursif.
    Graph::Node *m_jocker; ///< Représente la pièce jocker pour l'état final.
    Matrix<Graph::Node*> m_boardMatrix; ///< Représente le tableau de jeu. Utile pour l'accès en ligne/colonne.
    List::Node<Graph::Node*> *m_pieces; ///< Représente les pièces du jeu.
    Vector<Graph::Node*> m_index; ///< Permet de faire la liaison entre un état et les pièces.
    State m_initialState; ///< Représente l'état initial du jeu.
    State m_finalState; ///< Représente l'état final du jeu. Attention : ce n'est pas un état valide. Il ne faut tenir compte que du numéro des pièces, car il peut arriver qu'un noeud de pièce soit à plusieurs endroits à la fois !
    unsigned int m_nbNodes; ///< Représente le nombre de cases existantes sur le plateau.
};

#endif // GAME_H
