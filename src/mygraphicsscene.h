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
 *  \brief Associe la matrice chargée dans le xml à l'interface graphique.
 *  \param g La matrice chargée
 */
    void associateGame(Game *g);

/*!
 * \brief affiche la matrice chargée
 */
    void displayMatrix();

    void addPiecesInitialState();

    void addPiecesFinalState();
    void setState(const State *state);

    QColor createRainBow(unsigned int x);

    void createBrushs();

/*!
 *  \brief La fonction callResize va appeler le redimmensionage de ui->graphicsView en fonction du contenu de la matrice.
*/
    void callResize();

    void clear();


    ~MyGraphicsScene();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void sendResize(int w, int h);
    void sendPositions(QPointF *init,QPointF *final);

private:
    const State *m_currentState;
    QGraphicsRectItem ***m_items;
    QGraphicsSimpleTextItem ***m_texteItems; //affiche du texte dans les cases
    int m_HSize,m_WSize,m_BSize,m_caseHSize,m_caseWSize; ///< taille de hauteur, largeur et de base
    int m_baseRectSize;
    Game *m_game;
    QPixmap m_vide,m_base, m_rouge, m_bleu;
    QBrush **m_brushs; ///< Différents qpainter du jeu
    QPointF *m_initialPos, *m_finalPos;///< sauvegarde des positions initiales et finales de déplacement
};

#endif // MYGRAPHICSSCENE_H
