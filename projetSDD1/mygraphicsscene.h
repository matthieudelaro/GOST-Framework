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
    MyGraphicsScene(Matrix<Graph::Node*>& m);
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
};

#endif // MYGRAPHICSSCENE_H
