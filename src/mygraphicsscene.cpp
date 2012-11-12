#include <QPixmap>
#include <QPointF>
#include <QDebug>
#include "mygraphicsscene.h"

int myMax(int x,int y){return(x>y)?x:y;}

MyGraphicsScene::MyGraphicsScene() : m_matrix()
{
    m_matrix = NULL;
    m_itemTest = NULL;
    m_base.load("../res/img/case.png");
    m_vide.load("../res/img/vide.png");
    m_HSize = 0;
    m_WSize = 0;
    m_BSize = 0;
}

void MyGraphicsScene::associateMatrix(Matrix<Graph::Node *> *m)
{
    //on associe notre attribut au pointeur de la classe game
    m_matrix = m;
}

void MyGraphicsScene::displayMatrix()
{
    m_HSize = m_matrix->getHeight();
    m_WSize = m_matrix->getWidth();
    m_BSize = myMax(m_HSize,m_WSize);

    m_itemTest = new QGraphicsPixmapItem**[m_HSize];
    for(int i = 0; i < m_HSize; i ++)
    {
        m_itemTest[i] = new QGraphicsPixmapItem*[m_HSize];
        for(int j = 0; j < m_WSize; j ++)
        {
            if(m_matrix->operator ()(i,j) == NULL)
                m_itemTest[i][j] = new QGraphicsPixmapItem(m_vide);
            else
                m_itemTest[i][j] = new QGraphicsPixmapItem(m_base);
            m_itemTest[i][j]->setScale(1./(m_BSize)); //OK pour la redimension
            this->addItem(m_itemTest[i][j]);
            m_itemTest[i][j]->setPos(j*(500/m_BSize),i*(500/m_BSize)); //problèmes de position, plus la pièce est petite et plus elle se décale vers la gauche
        }
    }
}

void MyGraphicsScene::callResize()
{
    emit sendResize(m_WSize*(500/m_BSize)+3,m_HSize*(500/m_BSize)+3);
}

MyGraphicsScene::~MyGraphicsScene()
{
    if(m_itemTest != NULL)
    {
        for(int i = 0; i < m_HSize; i ++)
        {
            for(int j = 0; j < m_WSize; j ++)
                delete m_itemTest[i][j];
            delete m_itemTest[i];
        }
    }

    delete m_itemTest;

    //if(m_matrix != NULL)
    //   delete m_matrix;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //comme test permet juste de récupérer la position de la sourie au clic (ou en continu) et de trouver la case du tableau correspondante
    QPointF pos = mouseEvent->lastScenePos();
    QPointF tab;
    tab.setX((int)(pos.x()/(500*(1./m_BSize))));
    tab.setY((int)(pos.y()/(500*(1./m_BSize))));
    qDebug() << pos << tab;
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->lastScenePos();
    QPointF tab;
    tab.setX((int)(pos.x()/(500*(1./m_BSize))));
    tab.setY((int)(pos.y()/(500*(1./m_BSize))));
    qDebug() << pos << tab;
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //pour l'instant quedal
}

