#ifndef GAME_H
#define GAME_H

#include <QDomDocument>

#include "generalGame.h"

/*! \brief Gère le contenu du jeu : le plateau, les pièces et les règles.
 *
 *  La classe Game est un exemple d'utilisation du framework GOST.
 *  Elle limite les possibilités de jeux aux jeux 2D à carrée, mais implémente
 *  en contre-partie la prise en charge de jeux dynamique, via les fichiers xml
 *  ou le format texte.
 */
class Game : public Gost::GeneralGame
{
public:
    Game() : GeneralGame(), m_nbPieces(0) {}

    /*!
     * \brief Permet de charger un jeu au format xml.
     * \param xml Le fichier à charger.
     * \param error Si error est différent de NULL, alors error est utilisé pour renvoyer les erreurs de lecture du fichier xml
     * \return true si le chargement a été effectué avec succès, false sinon.
     */
    bool load(QDomDocument &xml, QString *error = NULL);

    /*!
     * \brief Permet de charger un jeu au format texte.
     *
     *  Cette fonction génère un xml à partir de file, et appel Game::load(QDomDocument &xml, QString *error = NULL) avec.
     *
     * \param file Le fichier à charger.
     * \param error Si error est différent de NULL, alors error est utilisé pour renvoyer les erreurs de lecture du fichier xml
     * \return true si le chargement a été effectué avec succès, false sinon.
     */
    bool load(const QString &file, QString *error = NULL);

    /*! \return La Matrix du plateau.
     */
    inline Gost::Matrix<Gost::Graph::Node*>* getBoardMatrix() {return &m_boardMatrix;}

    /*! \brief Effectue la liaison entre des coordonnées 2D et un noeud du plateau.
     *
     *  \param line La ligne dont on veut obtenir le noeud correspondant.
     *  \param column La colonne dont on veut obtenir le noeud correspondant.
     *  \return Un pointeur vers le noeud correspondant.
     */
    inline const Gost::Graph::Node* getBoardNode(unsigned int line, unsigned int column) const { return m_boardMatrix.getConst(line, column);}

    /*! \brief Effectue la liaison entre une position sur le plateau, et le noeud de la pièce qui l'occupe.
     *
     *  \param line Ordonnée de la position.
     *  \param column Abscisse de la position.
     *  \param state L'état du jeu auquel on souhaite obtenir le noeud de la pièce.
     *  \return Un pointeur vers le noeud de la pièce qui occupe boardNode.
     */
    const Gost::Graph::Node* getPieceNode(unsigned int line, unsigned int column, const Gost::State &state) const;

    /*!
     *  \return Le nombre de pièces du jeu.
     */
    virtual inline unsigned int getNbPieces() const {return m_nbPieces;}

    /*! \brief Vide le jeu.
     */
    virtual void clear();

    virtual ~Game();///< Le destructeur.

protected :
    Gost::Matrix<Gost::Graph::Node*> m_boardMatrix; ///< Représente le tableau de jeu. Utile pour l'accès en ligne/colonne.
    /*!
     * \brief Représente le nombre de pièces.
     *
     * m_nbPieces est calculé une seule fois, lors du chargement du jeu, pour éviter parcourir la liste des pièces à chaque fois
     * que l'on cherche cette information.
     * Ne pas confondre le nombre de pièces, et le nombre de cases.
     *
     * \see Gost::GeneralGame::m_nbNodes
     */
    unsigned int m_nbPieces;
};

#endif // GAME_H
