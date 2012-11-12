#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_scene = new MyGraphicsScene();
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->graphicsView->setMouseTracking(true);

    this->setCentralWidget(ui->centralWidget);

    //Appel de la fonction resize qui redimmensionne la GUI en fonction de la graphicsView
    QObject::connect(m_scene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

    //Connection des bouttons aux slots associés
    QObject::connect(ui->displayButton,SIGNAL(clicked()),this,SLOT(callDisplay()));
    QObject::connect(ui->associateButton,SIGNAL(clicked()),this,SLOT(callAssociateMatrix()));
    QObject::connect(ui->quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    //on empèche de pouvoir afficher la matrice sans avoir chargé le jeu
    ui->displayButton->setDisabled(true);
}

void MainWindow::loadXMLFile(QDomDocument &xml)
{
    m_game.load(xml);
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

void MainWindow::callAssociateMatrix()
{
    m_scene->associateMatrix(m_game.getBoardMatrix());

    //on empèche de refaire une association mais on autorise l'affichage
    ui->associateButton->setDisabled(true);
    ui->displayButton->setDisabled(false);
}

void MainWindow::callDisplay()
{
    ui->displayButton->setDisabled(true);
   //faire un test pour vérifier que la matrice à bien été associée
    m_scene->displayMatrix();
    m_scene->callResize();
}
