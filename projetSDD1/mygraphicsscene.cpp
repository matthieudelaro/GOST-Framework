#include <QPixmap>
#include <QPointF>
#include <QDebug>
#include "mygraphicsscene.h"

int myMax(int x,int y){return(x>y)?x:y;}

MyGraphicsScene::MyGraphicsScene()
{
    //on charge les images, que ça à faire ?
    // les mettres ailleurs ?
    itemTest = NULL;
    base = QPixmap("../res/case.png");
    vide = QPixmap("../res/vide.png");
}

void MyGraphicsScene::callResize()
{
    //ici très beau : pas y toucher
    emit sendResize(TL*(500/T)+3,TH*(500/T)+3);
}

void MyGraphicsScene::setMatrix(Matrix<Graph::Node *> &m)
{
    TH = m.getHeight();
    TL = m.getWidth();
    T = myMax(TH,TL);

    qDebug() << TL << TH;

    itemTest = new QGraphicsPixmapItem**[TH];
    for(int i = 0; i < TH; i ++)
    {
        itemTest[i] = new QGraphicsPixmapItem*[TL];
        for(int j = 0; j < TL; j ++)
        {
            if(m(i,j) == NULL)
                itemTest[i][j] = new QGraphicsPixmapItem(vide);
            else
                itemTest[i][j] = new QGraphicsPixmapItem(base);
            itemTest[i][j]->setScale(1./(T)); //OK pour la redimension
            this->addItem(itemTest[i][j]);
            itemTest[i][j]->setPos(j*(500/T),i*(500/T));
        }
    }
}

MyGraphicsScene::~MyGraphicsScene()
{
    if(itemTest)
    {
        for(int i = 0; i < TH; i ++)
        {
            for(int j = 0; j < TL; j ++)
                delete itemTest[i][j];
            delete itemTest[i];
        }
        delete itemTest;
    }
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
