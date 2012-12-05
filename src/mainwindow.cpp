#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentState = NULL;
    m_history = NULL;
    m_movesNumber = 0;

    m_xmlChoiceWindow = NULL;

    m_scene = NULL;

    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->graphicsView->setMouseTracking(true);

    this->setCentralWidget(ui->centralWidget);

    ui->actionAfficher_Fin->setShortcut(QKeySequence("Ctrl+f"));

    QList<QKeySequence> raccourcisChoixJeu;
    raccourcisChoixJeu.push_back(QKeySequence("Ctrl+o"));//o comme ouvrir
    raccourcisChoixJeu.push_back(QKeySequence("Ctrl+c"));//c comme choisir. Plus accessible pour la main gauche
    ui->actionChoixJeu->setShortcuts(raccourcisChoixJeu);

    finalStateWindows = NULL;

    QObject::connect(ui->actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
    QObject::connect(ui->actionChoixJeu,SIGNAL(triggered()),this,SLOT(callChoiceXmlFile()));
    QObject::connect(ui->actionAnnuler,SIGNAL(triggered()),this,SLOT(cancel()));
}

bool MainWindow::loadGameFromXml(QDomDocument &xml)
{
    if(m_game.load(xml))
    {
        if(m_scene)
        {
            delete m_scene;
            m_scene = NULL;
        }
        if(m_currentState)
        {
            delete m_currentState;
            m_currentState = NULL;
        }
        if(finalStateWindows)
        {
            delete finalStateWindows;
            finalStateWindows = NULL;
        }
        List::clearDelete(m_history);
        m_movesNumber = 0;

        m_scene = new MyGraphicsScene();
        ui->graphicsView->setScene(m_scene);

        //Appel de la fonction resize qui redimensionne la GUI en fonction de la graphicsView
        QObject::connect(m_scene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

        //appelle de la fonction qui vérifie si le déplacement est bon et récupérer la pièce correspondante
        QObject::connect(m_scene,SIGNAL(sendPositions(QPointF*,QPointF*)),this,SLOT(callIAPossibleMove(QPointF*,QPointF*)));

        m_currentState = m_game.getInitialStateCopy();
        m_scene->associateGame(&m_game);

        //m_scene->displayMatrix();
        m_scene->callResize();
        m_scene->setState(m_currentState);

        finalStateWindows = new EndWindow;

        QObject::connect(ui->actionAfficher_Fin,SIGNAL(triggered()),finalStateWindows,SLOT(show()));

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

    if(finalStateWindows)
        delete finalStateWindows;

    List::clearDelete(m_history);
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
            if(m_history && (*newState) == (*(m_history->info)))//si on fait marche arrière
            {
                cancel();
            }
            else
            {
                List::push_front(m_currentState, m_history);
                m_currentState = newState;
                m_movesNumber++;
                m_scene->setState(m_currentState);
            }
            if(IA::isEnd(*newState,m_game.getFinalState(),&m_game))
                QMessageBox::information(NULL,"Fin du jeu",QString::fromUtf8("Bien joué"));
        }
    }
}

void MainWindow::cancel()
{
    if(m_history)//si il y a des coups précédents
    {
        m_currentState = m_history->info;
        List::pop_front(m_history);
        if(m_movesNumber > 0)//protection contre l'overflow !
            m_movesNumber--;
    }
    m_scene->setState(m_currentState);
}

