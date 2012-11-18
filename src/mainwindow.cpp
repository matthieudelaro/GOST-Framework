#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_xmlChoiceWindow = NULL;

    m_scene = new MyGraphicsScene();
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->graphicsView->setMouseTracking(true);

    this->setCentralWidget(ui->centralWidget);

    //Appel de la fonction resize qui redimmensionne la GUI en fonction de la graphicsView
    QObject::connect(m_scene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

    //Connection des bouttons aux slots associés
    QObject::connect(ui->displayButton,SIGNAL(clicked()),this,SLOT(callDisplay()));
    QObject::connect(ui->loadGameButton,SIGNAL(clicked()),this,SLOT(callLoadGameFromXml()));
    QObject::connect(ui->associateButton,SIGNAL(clicked()),this,SLOT(callAssociateMatrix()));
    QObject::connect(ui->actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
    QObject::connect(ui->actionChoixJeu,SIGNAL(triggered()),this,SLOT(callChoiceXmlFile()));

    //on empèche de pouvoir afficher la matrice sans avoir chargé le jeu
    ui->displayButton->setDisabled(true);
}

void MainWindow::loadGameFromXml(QDomDocument &xml)
{
    m_game.load(xml);

    //méthode d'affichage du numéro des pièces dans l'index (donc ça n'affiche pas les void, seulement les free et les pièces)
    qDebug() << "Initial State :";
    for(unsigned int index = 0; index < m_game.getNbNodes(); ++index)
    {
        Graph::Node *node = m_game.getInitialState()[index];
        if(node)
            qDebug() << node->info;
        else
            qDebug() << "aucune piece";
    }
}

int MainWindow::loadXmlFromPath(QString path)
{
    QDir curr(QDir::currentPath());

    QString relativePath = curr.relativeFilePath(path);
    qDebug() << relativePath;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return EXIT_FAILURE;
    if (!m_XMLFileChosed.setContent(&file))
    {
        file.close(); // établit le document XML à partir des données du fichier (hiérarchie, etc.)
        return EXIT_FAILURE;
    }
    file.close();

    return 0; //tutti bueno
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_scene;
    if(m_xmlChoiceWindow)
        delete m_xmlChoiceWindow;
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

void MainWindow::callChoiceXmlFile()
{
    m_xmlChoiceWindow = new XmlFileChoice;
    QObject::connect(m_xmlChoiceWindow,SIGNAL(returnSelectedPath(QString)),this,SLOT(saveSelectedPathFromXml(QString)));
    m_xmlChoiceWindow->show();
}

void MainWindow::saveSelectedPathFromXml(QString path)
{
    m_loadedPath = path;
    m_xmlChoiceWindow->close();
    qDebug() << m_loadedPath;
    loadXmlFromPath(m_loadedPath);
}

void MainWindow::callLoadGameFromXml()
{
    loadGameFromXml(m_XMLFileChosed);
}

