#include "historicalwindow.h"
#include "ui_historicalwindow.h"
#include <QScrollBar>

using namespace Gost;

HistoricalWindow::HistoricalWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoricalWindow)
{
    ui->setupUi(this);

    m_scenes = NULL;
    m_views = NULL;

    m_verticalLayout = new QVBoxLayout;

    ui->scrollAreaWidgetContents->setLayout(m_verticalLayout);
    QObject::connect(ui->scrollArea->verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(moveScrollBarToBottom(int, int)));
}

void HistoricalWindow::displayGameHistory(List::Node<const State *> * possibleStates, Game &game, bool oppositeOrder)
{
    List::clearDelete(m_scenes);
    List::clearDelete(m_views);

    addStates(possibleStates, game, oppositeOrder);
}

HistoricalWindow::~HistoricalWindow()
{
    delete ui;
}

void HistoricalWindow::moveScrollBarToBottom(int min, int max)
{
    Q_UNUSED(min);
    ui->scrollArea->verticalScrollBar()->setValue(max);
}

void HistoricalWindow::addStates(List::Node<const State *> *possibleStates, Game &game, bool oppositeOrder)
{
    if(!possibleStates)
        return;

    if(oppositeOrder)
        addStates(possibleStates->next, game, oppositeOrder);

    //pour changer l'ordre de la liste, il suffit d'ajouter les éléments de la liste de façon récursive, et d'inverse l'ajout et le passage à l'élément suivant :)
    MyGraphicsScene * tmpScene = new MyGraphicsScene(250);
    QGraphicsView * tmpView = new QGraphicsView();

    tmpView->setScene(tmpScene);
    tmpView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    tmpView->setAcceptDrops(false);

    tmpScene->associateGame(&game);
    tmpScene->setState(possibleStates->info);

    tmpView->setFixedSize(tmpScene->getWSize()*(tmpScene->getBaseRectSize()/tmpScene->getBSize())+3,tmpScene->getHSize()*(tmpScene->getBaseRectSize()/tmpScene->getBSize())+3);

    List::push_front(tmpScene,m_scenes);
    List::push_front(tmpView,m_views);
    m_verticalLayout->addWidget(tmpView);

    if(!oppositeOrder)
        addStates(possibleStates->next, game, oppositeOrder);
}
