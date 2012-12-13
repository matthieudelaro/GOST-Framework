#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QColor>
#include <QPainter>
#include <QPointF>
#include "game.h"
#include "graph.h"

/*!
 *  \brief La classe MyGraphicsScene gère l'affichage graphique du jeu.
 *  \
 */
class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /*!
     * \brief Le constructeur définit la taille à 500 par défaut.
     * \param baseRectSize
     */
    MyGraphicsScene(int baseRectSize = 500);

    /*!
     * \brief Permet de changer le jeu affiché.
     * \param g Le nouveau jeu à afficher.
     */
    void associateGame(Game *g);

    /*!
     * \brief Change l'état du jeu affiché.
     * \param state Le nouvel état que l'on veut afficher.
     */
    void setState(const State *state);

    /*!
     * \brief Génère les couleurs de l'arc-en-ciel, et renvoie celle correspondant à x.
     *
     *  Cette fonction sert à générer des couleurs différents pour toutes les pièces du jeu, quel que soit leur nombre.
     *
     * \param x La couleur que l'on souhaite obtenir.
     * \return Une couleur de l'ar-en-ciel.
     */
    QColor createRainBow(unsigned int x);

    /*!
     * \brief Génère la palette de couleur utilisée pour les éléments du jeu.
     */
    void createBrushs();

    /*!
     *  \brief Appelle le redimensionnement de ui->graphicsView en fonction du contenu de la matrice.
     */
    void callResize();

    /*!
     * \brief Simple accesseur.
     * \return m_baseRectSize
     */
    unsigned int getBaseRectSize() {return m_baseRectSize;}

    /*!
     * \brief Simple accesseur.
     * \return m_WSize
     */
    unsigned int getWSize() {return m_WSize;}

    /*!
     * \brief Simple accesseur.
     * \return m_HSize
     */
    unsigned int getHSize() {return m_HSize;}

    /*!
     * \brief Simple accesseur.
     * \return m_BSize
     */
    unsigned int getBSize() {return m_BSize;}

    ~MyGraphicsScene();///< Libère m_initialPos et m_finalPos

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);///< Permet juste de récupérer la position de la souris au clic (ou en continu) et de trouver la case du tableau correspondante
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);///< Permet d'appeler le changement d'état lorsque l'utilisateur essaye de bouger une pièce.

signals:
    /*!
     *  \brief Appelle le redimensionnement de ui->graphicsView en fonction du contenu de la matrice.
     */
    void sendResize(int w, int h);

    /*!
     * \brief Appel le changement d'état lorsque l'utilisateur essaye de bouger une pièce.
     * \param Le point de départ du mouvement
     * \param Le point d'arrivée du mouvement
     */
    void sendPositions(QPointF *init,QPointF *final);

protected:
    const State *m_currentState;
    Matrix<QGraphicsRectItem* > m_items;
    Matrix<QGraphicsSimpleTextItem* > m_texteItems; ///< affiche du texte dans les cases
    int m_HSize,m_WSize,m_BSize,m_caseHSize,m_caseWSize; ///< taille de hauteur, largeur et de base
    int m_baseRectSize;
    Game *m_game;
    Vector<QBrush *> m_brushs; ///< Différents qpainter du jeu
    QPointF *m_initialPos, *m_finalPos;///< sauvegarde des positions initiales et finales de déplacement
};

#endif // MYGRAPHICSSCENE_H
