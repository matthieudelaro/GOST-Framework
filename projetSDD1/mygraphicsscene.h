#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QPixmap>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "matrix.h"
#include "graph.h"

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyGraphicsScene();
    void callResize();
    void setMatrix(Matrix<Graph::Node*>& m);
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
    QPixmap base,vide;
};

#endif // MYGRAPHICSSCENE_H
