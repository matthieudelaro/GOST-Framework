#include "historicalwindow.h"
#include "ui_historicalwindow.h"
#include <QScrollBar>

HistoricalWindow::HistoricalWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoricalWindow)
{
    ui->setupUi(this);

    m_scenes = NULL;
    m_views = NULL;

    m_verticalLayout = new QVBoxLayout;

    ui->scrollAreaWidgetContents->setLayout(m_verticalLayout);
}

void HistoricalWindow::displayGameHistory(List::Node<const State *> * possibleStates, Game &game, bool oppositeOrder)
{
    List::clearDelete(m_scenes);
    List::clearDelete(m_views);

    addStates(possibleStates, game, oppositeOrder);
    //ui->scrollArea->ensureVisible(0, 1000000);//je voudrais que la fenêtre scroll jusqu'au dernier élément, mais je n'y arrive pas
    //ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum()*2);//ne vas pas exactement jusqu'au bout. S'arrête à l'avant derner état.
}

HistoricalWindow::~HistoricalWindow()
{
    delete ui;
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

    //ui->scrollArea->ensureWidgetVisible(tmpView);
    //ui->scrollArea->ensureVisible(0, 1000000);//je voudrais que la fenêtre scroll jusqu'au dernier élément, mais je n'y arrive pas
}
