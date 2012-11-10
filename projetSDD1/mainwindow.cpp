#include "mainwindow.h"
#include "matrix.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(Game& g,QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    Matrix<Graph::Node*> toView = g.getBoardMatrix();//on récupère la matrice du jeu

    ui->setupUi(this);

    myScene = new MyGraphicsScene(toView);
    ui->graphicsView->setScene(myScene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->graphicsView->setMouseTracking(true);

    this->setCentralWidget(ui->centralWidget);

    QObject::connect(myScene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

    myScene->callResize();
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
