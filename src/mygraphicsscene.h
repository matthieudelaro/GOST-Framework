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
    \brief La classe MyGraphicsScene gère l'affichage graphique du jeu.
*/

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyGraphicsScene(int baseRectSize = 500);

/*!
 *  \brief associe la matrice garchée dans le xml à l'interface graphique
 *  \param g est la matrice chargée
 */
    void associateGame(Game *g);

/*!
 * \brief affiche la matrice chargée
 */
    void displayMatrix();

    void addPiecesInitialState();

    void addPiecesFinalState();

    void createBrushs();

/*!
 *  \brief La fonction callResize va appeler le redimmensionage de ui->graphicsView en fonction du contenu de la matrice.
*/
    void callResize();


    ~MyGraphicsScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void sendResize(int w, int h);
    void sendPositions(QPointF init,QPointF final);

private:
    QGraphicsRectItem ***m_itemBoard; ///< Les cases du plateau
    QGraphicsRectItem **m_itemPieces; ///< Les pièces de jeu
    int m_HSize,m_WSize,m_BSize; ///< taille de hauteur, largeur et de base
    int m_baseRectSize;
    Game *m_game;
    QPixmap m_vide,m_base, m_rouge, m_bleu;
    QBrush **m_brushs; ///< Différents qpainter du jeu
    QColor *m_colorList;
    QPointF initialPos, finalPos;
};

#endif // MYGRAPHICSSCENE_H
