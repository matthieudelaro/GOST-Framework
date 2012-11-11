#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    m_game = Game();
    /*faire une initialisation pour game du type
    game = Game();

    puis ensuite dans  loadXMLFile() :
    game.SetXmlFile()
    */
    ui->setupUi(this);

    m_scene = new MyGraphicsScene();
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->graphicsView->setMouseTracking(true);

    this->setCentralWidget(ui->centralWidget);

    QObject::connect(m_scene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

    m_scene->callResize();
}

void MainWindow::loadXMLFile(QDomDocument &xml)
{
    m_game.load(xml);
    //on associe tout de suite après la matrice à la scene
    m_scene->associateMatrix(m_game.getBoardMatrix());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_scene;
}

void MainWindow::resize(int w, int h)
{
    ui->graphicsView->setFixedSize(w,h);
    qDebug() << "test" << ui->graphicsView->size();
}
