#include <QPixmap>
#include <QPointF>
#include <QDebug>
#include "mygraphicsscene.h"

int myMax(int x,int y){return(x>y)?x:y;}

MyGraphicsScene::MyGraphicsScene(int w, int h)
{
    TH = h;
    TL = w;
    T = myMax(TH,TL);
    QPixmap base = QPixmap("../res/case.png");
    itemTest = new QGraphicsPixmapItem**[TH];
    for(int i = 0; i < TH; i ++)
    {
        itemTest[i] = new QGraphicsPixmapItem*[TL];
        for(int j = 0; j < TL; j ++)
        {
            itemTest[i][j] = new QGraphicsPixmapItem(base);
            itemTest[i][j]->setScale(1./(T)); //OK pour la redimension
            this->addItem(itemTest[i][j]);
            itemTest[i][j]->setPos(j*(500/T),i*(500/T)); //problèmes de position, plus la pièce est petite et plus elle se décale vers la gauche
        }
    }
}

void MyGraphicsScene::callResize()
{
    emit sendResize(TL*(500/T)+3,TH*(500/T)+3);
}

MyGraphicsScene::~MyGraphicsScene()
{
    for(int i = 0; i < TH; i ++)
    {
        for(int j = 0; j < TL; j ++)
            delete itemTest[i][j];
        delete itemTest[i];
    }
    delete itemTest;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //comme test permet juste de récupérer la position de la sourie au clic (ou en continu) et de trouver la case du tableau correspondante
    QPointF pos = mouseEvent->lastScenePos();
    QPointF tab;
    tab.setX((int)(pos.x()/(500*(1./T))));
    tab.setY((int)(pos.y()/(500*(1./T))));
    qDebug() << pos << tab;
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->lastScenePos();
    QPointF tab;
    tab.setX((int)(pos.x()/(500*(1./T))));
    tab.setY((int)(pos.y()/(500*(1./T))));
    qDebug() << pos << tab;
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //pour l'instant quedal
}
