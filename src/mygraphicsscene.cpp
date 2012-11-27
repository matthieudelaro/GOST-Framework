#include <QPixmap>
#include <QPointF>
#include <QDebug>
#include "mygraphicsscene.h"

int myMax(int x,int y){return(x>y)?x:y;}

MyGraphicsScene::MyGraphicsScene(int baseRectSize)
{
    m_game = NULL;
    m_itemBoard = NULL;
    m_itemPieces = NULL;
    m_brushs = NULL;
    m_HSize = 0;
    m_WSize = 0;
    m_BSize = 0;
    m_baseRectSize = baseRectSize;

    m_colorList =  new QColor[4];
    m_colorList[0] = QColor::fromRgb(204,204,204);
    m_colorList[1] = QColor::fromRgb(51,51,51);
    m_colorList[2] = QColor::fromRgb(255,0,0);
    m_colorList[3] = QColor::fromRgb(0,0,255);
   // m_colorList[4] = QColor::fromRgb(0,255,0);
}

void MyGraphicsScene::associateGame(Game *g)
{
    //on associe notre attribut au pointeur de la classe game
    m_game = g;
}

void MyGraphicsScene::displayMatrix()
{
    m_HSize = m_game->getBoardMatrix()->getHeight();
    m_WSize = m_game->getBoardMatrix()->getWidth();
    m_BSize = myMax(m_HSize,m_WSize);

    createBrushs();

    m_itemBoard = new QGraphicsRectItem**[m_HSize];
    for(int i = 0; i < m_HSize; i ++)
    {
        m_itemBoard[i] = new QGraphicsRectItem*[m_HSize];
        for(int j = 0; j < m_WSize; j ++)
        {
            m_itemBoard[i][j] =  new QGraphicsRectItem(0,0,m_baseRectSize,m_baseRectSize);
            if(m_game->getBoardMatrix()->operator ()(i,j) == NULL)
                m_itemBoard[i][j]->setBrush(*m_brushs[0]);
            else
                m_itemBoard[i][j]->setBrush(*m_brushs[1]);
            m_itemBoard[i][j]->setScale(1./(m_BSize)); //OK pour la redimension
            this->addItem(m_itemBoard[i][j]);
            m_itemBoard[i][j]->setPos(j*(m_baseRectSize/m_BSize),i*(m_baseRectSize/m_BSize)); //problèmes de position, plus la pièce est petite et plus elle se décale vers la gauche
        }
    }
}

void MyGraphicsScene::addPiecesInitialState()
{
    m_itemPieces = new QGraphicsRectItem*[m_game->getNbNodes()];
    int inc = 0; //sert à faire de l'affichage, pour compter le nombre de "blancs"
    for(int i = 0; i < m_game->getNbNodes(); i++)
    {
        Graph::Node *node = m_game->getInitialState()[i];

        if(m_game->getBoardMatrix()->operator [](i) == NULL)
            inc ++;

        if(node)
        {
            m_itemPieces[i] = new QGraphicsRectItem(0,0,m_baseRectSize,m_baseRectSize);
            m_itemPieces[i]->setBrush(*m_brushs[node->info + 1]);
            m_itemPieces[i]->setScale(1./(m_BSize)); //OK pour la redimension
            this->addItem(m_itemPieces[i]);
            m_itemPieces[i]->setPos((m_baseRectSize/m_BSize)*((i+inc)%m_game->getBoardMatrix()->getWidth()),(m_baseRectSize/m_BSize)*((i+inc)/m_game->getBoardMatrix()->getWidth()));
        }
    }
}

void MyGraphicsScene::addPiecesFinalState()
{
    qDebug() << "final final final";
    m_itemPieces = new QGraphicsRectItem*[m_game->getNbNodes()];
    int inc = 0; //sert à faire de l'affichage, pour compter le nombre de "blancs"
    for( int i = 0; i < m_game->getNbNodes(); i++)
    {
        Graph::Node *node = m_game->getFinalState()[i];

        if(m_game->getBoardMatrix()->operator [](i) == NULL)
            inc ++;

        if(node)
        {
            m_itemPieces[i] = new QGraphicsRectItem(0,0,m_baseRectSize,m_baseRectSize);
            m_itemPieces[i]->setBrush(*m_brushs[node->info + 1]);

            m_itemPieces[i]->setScale(1./(m_BSize)); //OK pour la redimension
            this->addItem(m_itemPieces[i]);
            m_itemPieces[i]->setPos((m_baseRectSize/m_BSize)*((i+inc)%m_game->getBoardMatrix()->getWidth()),(m_baseRectSize/m_BSize)*((i+inc)/m_game->getBoardMatrix()->getWidth()));
        }
    }
}

void MyGraphicsScene::createBrushs()
{
    // i = 0 : empty background
    // i = 1 : board background
    // i > 1 : piece's color
    //
    m_brushs =  new QBrush*[4];
    for(int i = 0; i < 4; i ++)
        m_brushs[i] = new QBrush(m_colorList[i]);
}

void MyGraphicsScene::callResize()
{
    emit sendResize(m_WSize*(m_baseRectSize/m_BSize)+3,m_HSize*(m_baseRectSize/m_BSize)+3);
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
        delete m_itemBoard;
    }

    if(m_brushs)
    {
        for(int j = 0; j < 4; j ++)
            delete m_brushs[j];
        delete m_brushs;
    }
    delete [] m_colorList;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //comme test permet juste de récupérer la position de la sourie au clic (ou en continu) et de trouver la case du tableau correspondante
    QPointF pos = mouseEvent->lastScenePos();
    initialPos.setX((int)(pos.x()/(m_baseRectSize*(1./m_BSize))));
    initialPos.setY((int)(pos.y()/(m_baseRectSize*(1./m_BSize))));
}

void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->lastScenePos();
    finalPos.setX((int)(pos.x()/(m_baseRectSize*(1./m_BSize))));
    finalPos.setY((int)(pos.y()/(m_baseRectSize*(1./m_BSize))));
    emit sendPositions(initialPos,finalPos);
}

