#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    myScene = new MyGraphicsScene();
    ui->graphicsView->setScene(myScene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->graphicsView->setMouseTracking(true);

    this->setCentralWidget(ui->centralWidget);

    QObject::connect(myScene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));
    QObject::connect(ui->testButton,SIGNAL(clicked()),this,SLOT(callSetMatrix()));
    QObject::connect(ui->quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));
}

void MainWindow::setGame(Game &g)
{
    toView = g.getBoardMatrix();//on récupère la matrice du jeu
}

MainWindow::~MainWindow()
{
    delete ui;
    delete myScene;
}

void MainWindow::resize(int w, int h)
{
    ui->graphicsView->setFixedSize(w,h);
    qDebug() << "test" << ui->graphicsView->size();
}

void MainWindow::callSetMatrix()
{
    myScene->setMatrix(toView);
    myScene->callResize();
}
