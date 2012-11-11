#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "matrix.h"
#include "graph.h"

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyGraphicsScene(int w = 2 , int h = 2);
    void associateMatrix(const Matrix<Graph::Node*>& m);//on associe la matrice chargée dans le xml à l'interface graphique
    void displayMatrix(); //affiche la matrice nouvellement chargée
    void callResize();
    ~MyGraphicsScene();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void sendResize(int w, int h);

private:
    QGraphicsPixmapItem ***itemTest;
    int TL,TH,T;
    Matrix<Graph::Node*>& m_matrix;
};

#endif // MYGRAPHICSSCENE_H
