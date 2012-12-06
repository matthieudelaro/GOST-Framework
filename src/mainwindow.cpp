#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextStream>

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

    QList<QKeySequence> raccourcisChoixJeu;
    raccourcisChoixJeu.push_back(QKeySequence("Ctrl+o"));//o comme ouvrir
    raccourcisChoixJeu.push_back(QKeySequence("Ctrl+c"));//c comme choisir. Plus accessible pour la main gauche
    ui->actionChoixJeu->setShortcuts(raccourcisChoixJeu);

    m_finalStateWindow = NULL;
    m_historicalwindow = NULL;
    QObject::connect(ui->actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
    QObject::connect(ui->actionChoixJeu,SIGNAL(triggered()),this,SLOT(callChoiceGameFile()));
    QObject::connect(ui->actionAnnuler,SIGNAL(triggered()),this,SLOT(cancel()));
}

bool MainWindow::loadGameFromPath(QString &path, QString *error)
{
//    QString file;
//    file += "ABCX\n";
//    file += "D...\n";
//    file += "\n";
//    file += ".##X\n";
//    file += ".A#C\n";
//    if(m_game.load(file))

    //on prépare la sortie d'erreur
    QString* log;
    if(error)
        log = error;
    else
        log = new QString();

    QFile file(path);
    if(!file.exists())
    {
        *log += "Pas de fichier";
        return false;
    }

    if (!file.open(QIODevice::ReadOnly))
    {
        *log += "Pas de fichier lisible";
        return false;
    }

    QDomDocument xmlDomDocument;

    if(xmlDomDocument.setContent(&file))
    {
        if(!m_game.load(xmlDomDocument,log))
        {
            QMessageBox::information(this,"Erreur de chargement du QDomDocument",*log);
            file.close();
            return false;
        }
    }
    else
    {
        QTextStream *texteStream = new QTextStream(&file);
        QString toLoad = texteStream->readAll();
        qDebug() << toLoad;
        if(!m_game.load(toLoad,log))
        {
            QMessageBox::information(this,"Erreur de chargement du QString",*log);
            file.close();
            return false;
        }
    }
    file.close();

    //si existe pas , ok = false
    //sinon
    //  si chargement QDomDocument et chargement game xml, ok = true
    //  sinon
    //      si ! chargement game string , ok = false, sinon, ok = true


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
    if(m_finalStateWindow)
    {
        delete m_finalStateWindow;
        m_finalStateWindow = NULL;
    }
    if(m_historicalwindow)
    {
        delete m_historicalwindow;
        m_historicalwindow = NULL;
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

    m_finalStateWindow = new EndWindow;

    QObject::connect(ui->actionAfficher_Fin,SIGNAL(triggered()),m_finalStateWindow,SLOT(show()));

    m_finalStateWindow->display(m_game);


    m_historicalwindow = new HistoricalWindow;
    QObject::connect(ui->actionAfficher_l_Historique,SIGNAL(triggered()),m_historicalwindow,SLOT(show()));

    return true;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_scene;

    if(m_xmlChoiceWindow)
        delete m_xmlChoiceWindow;

    if(m_finalStateWindow)
        delete m_finalStateWindow;

    if(m_historicalwindow)
        delete m_historicalwindow;

    List::clearDelete(m_history);
}

void MainWindow::resize(int w, int h)
{
    ui->graphicsView->setFixedSize(w,h);
}


void MainWindow::callChoiceGameFile()
{
    m_xmlChoiceWindow = new XmlFileChoice;
    QObject::connect(m_xmlChoiceWindow,SIGNAL(returnSelectedPath(QString)),this,SLOT(saveSelectedPathFromXml(QString)));
    m_xmlChoiceWindow->show();
}

void MainWindow::saveSelectedPathFromXml(QString path)
{
    m_loadedPath = path;
    m_xmlChoiceWindow->close();
    loadGameFromPath(path);
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
            else
                showDifferentsPossibleStates();
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

void MainWindow::showDifferentsPossibleStates()
{
    qDebug() << "showPossibleState";
    List::Node<const State *>* possibleStates = IA::getPossibleMove(*m_currentState,m_game);

    m_historicalwindow->displayGameHistory(possibleStates,m_game);

    /*qDebug() << possibleStates;
    while(possibleStates)
    {
        QMessageBox::information(NULL,"","on recommence");
        m_scene->setState(m_currentState);
        QMessageBox::information(NULL,"","on avance");
        m_scene->setState(possibleStates->info);
        possibleStates = possibleStates->next;
    }*/
}

