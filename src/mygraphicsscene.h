#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include "matrix.h"
#include "graph.h"

/*!
    \brief La classe MyGraphicsScene gère l'affichage graphique du jeu.
*/

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyGraphicsScene();
    void associateMatrix(Matrix<Graph::Node *> *m);//on associe la matrice chargée dans le xml à l'interface graphique
    void displayMatrix(); //affiche la matrice nouvellement chargée

/*!
    \brief La fonction callResize va appeler le redimmensionage de ui->graphicsView en fonction du contenu de la matrice.
*/
    void callResize();
    ~MyGraphicsScene();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void sendResize(int w, int h);

private:
    QGraphicsPixmapItem ***m_itemTest;
    int m_HSize,m_WSize,m_BSize; //taille de hauteur, largeur et de base
    Matrix<Graph::Node*> *m_matrix;
    QPixmap m_vide,m_base;
};

#endif // MYGRAPHICSSCENE_H
