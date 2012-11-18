#include <QPixmap>
#include <QPointF>
#include <QDebug>
#include "mygraphicsscene.h"

int myMax(int x,int y){return(x>y)?x:y;}

MyGraphicsScene::MyGraphicsScene() : m_matrix()
{
    m_game = NULL;
    m_matrix = NULL;
    m_itemBoard = NULL;
    m_itemPieces = NULL;
    m_base.load("../res/img/case.png");
    m_vide.load("../res/img/vide.png");
    m_bleu.load("../res/img/bleu.png");
    m_rouge.load("../res/img/rouge.png");
    m_HSize = 0;
    m_WSize = 0;
    m_BSize = 0;
}

void MyGraphicsScene::associateGame(Game *g)
{
    //on associe notre attribut au pointeur de la classe game
    m_game = g;
    m_matrix = m_game->getBoardMatrix();
}

void MyGraphicsScene::displayMatrix()
{
    m_HSize = m_matrix->getHeight();
    m_WSize = m_matrix->getWidth();
    m_BSize = myMax(m_HSize,m_WSize);

    m_itemBoard = new QGraphicsPixmapItem**[m_HSize];
    for(int i = 0; i < m_HSize; i ++)
    {
        m_itemBoard[i] = new QGraphicsPixmapItem*[m_HSize];
        for(int j = 0; j < m_WSize; j ++)
        {
            if(m_matrix->operator ()(i,j) == NULL)
                m_itemBoard[i][j] = new QGraphicsPixmapItem(m_vide);
            else
                m_itemBoard[i][j] = new QGraphicsPixmapItem(m_base);
            m_itemBoard[i][j]->setScale(1./(m_BSize)); //OK pour la redimension
            this->addItem(m_itemBoard[i][j]);
            m_itemBoard[i][j]->setPos(j*(500/m_BSize),i*(500/m_BSize)); //problèmes de position, plus la pièce est petite et plus elle se décale vers la gauche
        }
    }
}

void MyGraphicsScene::addPieces()
{
    m_itemPieces = new QGraphicsPixmapItem*[m_game->getNbNodes()];
    int inc = 0; //sert à faire de l'affichage, pour compter le nombre de "blancs"
    for( int i = 0; i < m_game->getNbNodes(); i++)
    {
        Graph::Node *node = m_game->getInitialState()[i];
        if(m_matrix->operator [](i) == NULL)
            inc ++;
        if(node)
        {
            if(node->info == 1)
                m_itemPieces[i] = new QGraphicsPixmapItem(m_bleu);
            else
                m_itemPieces[i] = new QGraphicsPixmapItem(m_rouge);

            m_itemPieces[i]->setScale(1./(m_BSize)); //OK pour la redimension
            this->addItem(m_itemPieces[i]);
            m_itemPieces[i]->setPos((500/m_BSize)*((i+inc)%m_matrix->getWidth()),(500/m_BSize)*((i+inc)/m_matrix->getWidth()));
        }
    }
}

void MyGraphicsScene::callResize()
{
    emit sendResize(m_WSize*(500/m_BSize)+3,m_HSize*(500/m_BSize)+3);
}

MyGraphicsScene::~MyGraphicsScene()
{
    if(m_itemBoard != NULL)
    {
        for(int i = 0; i < m_HSize; i ++)
        {
            for(int j = 0; j < m_WSize; j ++)
                delete m_itemBoard[i][j];
            delete m_itemBoard[i];
        }
    }

    delete m_itemBoard;
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

