#include <QPixmap>
#include <QPointF>
#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include "mygraphicsscene.h"

inline int myMax(int x,int y){return(x>y)?x:y;}

MyGraphicsScene::MyGraphicsScene(int baseRectSize)
{
    m_game = NULL;
    m_HSize = 0;
    m_WSize = 0;
    m_BSize = 0;
    m_caseHSize = 0;
    m_caseWSize = 0;
    m_baseRectSize = baseRectSize;
    m_initialPos = new QPointF(0,0);
    m_finalPos = new QPointF(0,0);
}

void MyGraphicsScene::associateGame(Game *g)
{
    //on associe notre attribut au pointeur de la classe game
    m_game = g;
    createBrushs();

    //on initialise le plateau
    m_HSize = m_game->getBoardMatrix()->getHeight();
    m_WSize = m_game->getBoardMatrix()->getWidth();

    m_BSize = myMax(m_HSize,m_WSize);

    m_caseHSize = m_baseRectSize/m_BSize;
    m_caseWSize = m_baseRectSize/m_BSize;

    m_items.resize(m_game->getBoardMatrix()->getHeight(), m_game->getBoardMatrix()->getWidth());
    m_texteItems.resize(m_game->getBoardMatrix()->getHeight(), m_game->getBoardMatrix()->getWidth());

    for(unsigned int line = 0; line < m_game->getBoardMatrix()->getHeight(); ++line)
    {
        for(unsigned int column = 0; column < m_game->getBoardMatrix()->getWidth(); ++column)
        {
            m_items(line, column) = new QGraphicsRectItem(0, 0, m_caseWSize, m_caseHSize);

            m_texteItems(line, column) = new QGraphicsSimpleTextItem(m_items(line, column));
            m_texteItems(line, column)->setPos(m_caseHSize/2,m_caseWSize/2); //mettre le texte au centre mais pas très beau
            m_texteItems(line, column)->setScale((double)(m_baseRectSize)/400.0);

            if(m_game->getBoardNode(line, column))
            {
                m_items(line, column)->setBrush(*m_brushs[1]);
            }
            else
            {
                m_items(line, column)->setBrush(*m_brushs[0]);
            }
            m_items(line, column)->setPos(column*m_caseWSize, line*m_caseHSize);
            this->addItem(m_items(line, column));
        }
    }
    callResize();
}

void MyGraphicsScene::addPiecesInitialState()
{
    setState(&(m_game->getInitialState()));
}

void MyGraphicsScene::addPiecesFinalState()
{
    setState(&(m_game->getFinalState()));
}

void MyGraphicsScene::setState(const State *state)
{
    if(state)
    {
        for(unsigned int line = 0; line < m_game->getBoardMatrix()->getHeight(); ++line)
        {
            for(unsigned int column = 0; column < m_game->getBoardMatrix()->getWidth(); ++column)
            {
                if(m_game->getPieceNode(line,column,*state))
                {
                    m_items(line, column)->setBrush(*m_brushs[m_game->getPieceNode(line,column,*state)->info + 1]);
                    if(m_game->getPieceNode(line,column,*state)->info == 0)
                        m_texteItems(line, column)->setText("J");
                    else
                        m_texteItems(line, column)->setText(QString::number(m_game->getPieceNode(line,column,*state)->info));
                }
                else
                {
                    m_texteItems(line, column)->setText("");
                    if(m_game->getBoardNode(line,column))
                        m_items(line, column)->setBrush(*m_brushs[1]);
                    else
                        m_items(line, column)->setBrush(*m_brushs[0]);
                }
            }
        }
    }
}

QColor MyGraphicsScene::createRainBow(unsigned int x)
{
    QColor color;
    int r = 255;
    int g = 255;
    int b = 255;
    if(x < 255)
    {
        r = 255;
        g = x;
        b = 0;
    }
    else if(x >= 255 && x < 510)
    {
        r = 510-x;
        g = 255;
        b = 0;
    }
    else if(x >= 510 && x < 765)
    {
        r = 0;
        g = 255;
        b = x-510;
    }
    else if(x >= 765 && x < 1020)
    {
        r = 0;
        g = 1020-x;
        b = 255;
    }
    else if(x >= 1020 && x < 1275)
    {
        r = x-1020;
        g = 0;
        b = 255;
    }
    else if(x >= 1275 && x <= 1530)
    {
        r = 255;
        g = 0;
        b = 1530-x;
    }
    color.setRgb(r, g, b);
    return color;
}



void MyGraphicsScene::createBrushs()
{
    // i = 0 : empty background
    // i = 1 : board background
    // i > 1 : piece's color

    if(m_game)
    {
        unsigned int nbPieces = m_game->getNbPieces();

        m_brushs.resize(nbPieces+2);
        m_brushs[0] = new QBrush(QColor(200,200,200));
        m_brushs[1] = new QBrush(QColor(110,110,110));
        for(unsigned int i = 2; i < nbPieces + 2; i ++)
            m_brushs[i] = new QBrush(createRainBow(i*(1520/(nbPieces+2))));

    }
}
void MyGraphicsScene::callResize()
{
    if(m_game)
        emit sendResize(m_WSize*(m_baseRectSize/m_BSize)+3,m_HSize*(m_baseRectSize/m_BSize)+3);
}

MyGraphicsScene::~MyGraphicsScene()
{
    if(m_initialPos)
        delete m_initialPos;
    if(m_finalPos)
        delete m_finalPos;
}

void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    //comme test permet juste de récupérer la position de la sourie au clic (ou en continu) et de trouver la case du tableau correspondante
    QPointF pos = mouseEvent->lastScenePos();
    m_initialPos->setX((int)(pos.x()/(m_baseRectSize*(1./m_BSize))));
    m_initialPos->setY((int)(pos.y()/(m_baseRectSize*(1./m_BSize))));
}

void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QPointF pos = mouseEvent->lastScenePos();
    m_finalPos->setX((int)(pos.x()/(m_baseRectSize*(1./m_BSize))));
    m_finalPos->setY((int)(pos.y()/(m_baseRectSize*(1./m_BSize))));
    emit sendPositions(m_initialPos,m_finalPos);
}

