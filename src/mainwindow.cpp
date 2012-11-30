#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentState = NULL;

    m_xmlChoiceWindow = NULL;

    m_scene = new MyGraphicsScene();
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->graphicsView->setMouseTracking(true);

    this->setCentralWidget(ui->centralWidget);

    ui->actionChoixJeu->setShortcut(QKeySequence("Ctrl+o"));
    ui->actionAfficher_Fin->setShortcut(QKeySequence("Ctrl+f"));

    finalStateWindows = new EndWindow;

    QObject::connect(ui->actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
    QObject::connect(ui->actionChoixJeu,SIGNAL(triggered()),this,SLOT(callChoiceXmlFile()));

    QObject::connect(ui->actionAfficher_Fin,SIGNAL(triggered()),finalStateWindows,SLOT(show()));

    //Appel de la fonction resize qui redimensionne la GUI en fonction de la graphicsView
    QObject::connect(m_scene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

    //appelle de la fonction qui vérifie si le déplacement est bon et récupérer la pièce correspondante
    QObject::connect(m_scene,SIGNAL(sendPositions(QPointF*,QPointF*)),this,SLOT(callIAPossibleMove(QPointF*,QPointF*)));
}

bool MainWindow::loadGameFromXml(QDomDocument &xml)
{
    if(m_game.load(xml))
    {
        m_currentState = & m_game.getInitialState();
        m_scene->associateGame(&m_game);

        m_scene->displayMatrix();
        m_scene->callResize();
        m_scene->addPiecesInitialState();

        finalStateWindows->display(m_game);
        return true;
    }
    else
    {
        QMessageBox::critical(this, "Ouverture du jeu", "Le fichier ne respecte pas le format attendu");
        return false;
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

    delete finalStateWindows;
}

void MainWindow::resize(int w, int h)
{
    ui->graphicsView->setFixedSize(w,h);
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
    loadXmlFromPath(m_loadedPath);
    loadGameFromXml(m_XMLFileChosed);
}

void MainWindow::callIAPossibleMove(QPointF *init, QPointF *final)
{
    if(m_game.getBoardMatrix()->inRange(init->y(),init->x()) && m_game.getBoardMatrix()->inRange(final->y(),final->x()))
    {
        State *newState = IA::possibleMove(*m_currentState,
                                           m_game.getBoardMatrix()->get(init->y(),init->x()),
                                           m_game.getBoardMatrix()->get(final->y(),final->x()),
                                           m_game);
        if(newState)
        {
            qDebug() << "deplacement autorisé !";
            m_currentState = newState;
            m_scene->setState(m_currentState);
            finalStateWindows->display(m_game);
        }
    }
}

