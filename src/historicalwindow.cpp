#include "historicalwindow.h"
#include "ui_historicalwindow.h"

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

void HistoricalWindow::displayGameHistory(List::Node<const State *> * possibleStates, Game &game)
{
    List::Node<const State *> *it = possibleStates;

    List::clearDelete(m_scenes);
    List::clearDelete(m_views);

    while(it)
    {
        MyGraphicsScene * tmpScene = new MyGraphicsScene(250);
        QGraphicsView * tmpView = new QGraphicsView();

        tmpView->setScene(tmpScene);
        tmpView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
        tmpView->setAcceptDrops(false);

        tmpScene->associateGame(&game);
        tmpScene->setState(it->info);

        tmpView->setFixedSize(tmpScene->getWSize()*(tmpScene->getBaseRectSize()/tmpScene->getBSize())+3,tmpScene->getHSize()*(tmpScene->getBaseRectSize()/tmpScene->getBSize())+3);


        List::push_front(tmpScene,m_scenes);
        List::push_front(tmpView,m_views);
        m_verticalLayout->addWidget(tmpView);
        it = it->next;
    }
}

HistoricalWindow::~HistoricalWindow()
{
    delete ui;
}

void HistoricalWindow::resize(int w, int h)
{

}
