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

    m_labelMovesNumber = new QLabel(QString());
    ui->statusBar->addPermanentWidget(m_labelMovesNumber);

    m_finalStateWindow = NULL;
    m_historicalWindow = NULL;
    m_debugHistoricalwindow = NULL;
    QObject::connect(ui->actionQuitter,SIGNAL(triggered()),qApp,SLOT(quit()));
    QObject::connect(ui->actionChoixJeu,SIGNAL(triggered()),this,SLOT(callChoiceGameFile()));
    QObject::connect(ui->actionAnnuler,SIGNAL(triggered()),this,SLOT(cancel()));
    QObject::connect(ui->actionRefaire_le_dernier_coup,SIGNAL(triggered()),this,SLOT(redo()));
}

bool MainWindow::loadGameFromPath(QString &path, QString *error)
{
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

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        *log += "Pas de fichier lisible";
        return false;
    }

    QTextStream in(&file);
    QString toLoad = in.readAll();

    QDomDocument xmlDomDocument;

    if(xmlDomDocument.setContent(toLoad))
    {
        if(!m_game.load(xmlDomDocument,log))
        {
            QMessageBox::information(this, "Erreur de chargement du fichier", QString::fromUtf8(log->toStdString().c_str()));
            file.close();
            return false;
        }
    }
    else
    {
        if(!m_game.load(toLoad,log))
        {
            QMessageBox::information(this,"Erreur de chargement du QString",*log);
            file.close();
            return false;
        }
    }
    file.close();

    //si on arrive jusqu'ici, ça veut dire que le chargement du nouveau jeu a fonctionné.
    //on supprime l'ancien
    if(m_scene)
    {
        delete m_scene;
        m_scene = NULL;
    }
    if(m_finalStateWindow)
    {
        m_finalStateWindow->close();
        delete m_finalStateWindow;
        m_finalStateWindow = NULL;
    }
    if(m_historicalWindow)
    {
        m_historicalWindow->close();
        delete m_historicalWindow;
        m_historicalWindow = NULL;
    }
    if(m_debugHistoricalwindow)
    {
        m_debugHistoricalwindow->close();
        delete m_debugHistoricalwindow;
        m_debugHistoricalwindow = NULL;
    }
    List::clearDelete(m_history);
    m_currentState = NULL;
    m_movesNumber = 0;
    m_labelMovesNumber->setText(QString());

    //on met en place le nouveau jeu
    m_scene = new MyGraphicsScene();
    ui->graphicsView->setScene(m_scene);

    //Appel de la fonction resize qui redimensionne la GUI en fonction de la graphicsView
    QObject::connect(m_scene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

    //appelle de la fonction qui vérifie si le déplacement est bon et récupérer la pièce correspondante
    QObject::connect(m_scene,SIGNAL(sendPositions(QPointF*,QPointF*)),this,SLOT(callIAPossibleMove(QPointF*,QPointF*)));

    List::push_front<const State *>(m_game.getInitialStateCopy(), m_history);
    m_currentState = m_history;//Au début du jeu, l'état courant est le premier de l'historique
    m_scene->associateGame(&m_game);

    m_scene->callResize();

    m_finalStateWindow = new EndWindow;

    QObject::connect(ui->actionAfficher_Fin,SIGNAL(triggered()),m_finalStateWindow,SLOT(show()));

    m_finalStateWindow->display(m_game);

    m_historicalWindow = new HistoricalWindow;
    m_historicalWindow->setWindowTitle("Historique");
    m_debugHistoricalwindow = new HistoricalWindow;
    m_debugHistoricalwindow->setWindowTitle("Prochains coups possibles");
    QObject::connect(ui->actionAfficher_l_Historique,SIGNAL(triggered()),m_historicalWindow,SLOT(show()));
    QObject::connect(ui->actionDebug,SIGNAL(triggered()),m_debugHistoricalwindow,SLOT(show()));

    setState();
    return true;
}

void MainWindow::setState()
{
    m_scene->setState(m_currentState->info);
    m_historicalWindow->displayGameHistory(m_currentState, m_game, true);
    if(m_movesNumber == 1)
        m_labelMovesNumber->setText(QString("Vous avez fait %1 coup.").arg(m_movesNumber));
    else if(m_movesNumber > 1)
        m_labelMovesNumber->setText(QString("Vous avez fait %1 coups.").arg(m_movesNumber));
    else
        m_labelMovesNumber->setText(QString::fromUtf8("Début du jeu"));

    if(IA::isEnd(*(m_currentState->info),m_game.getFinalState(),&m_game))
        QMessageBox::information(NULL,"Fin du jeu",QString::fromUtf8("Bien joué"));
    else
        showDifferentsPossibleStates();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_scene;

    if(m_xmlChoiceWindow)
        delete m_xmlChoiceWindow;

    if(m_finalStateWindow)
    {
        m_finalStateWindow->close();
        delete m_finalStateWindow;
    }
    if(m_historicalWindow)
    {
        m_historicalWindow->close();
        delete m_historicalWindow;
    }
    if(m_debugHistoricalwindow)
    {
        m_debugHistoricalwindow->close();
        delete m_debugHistoricalwindow;
    }

    List::clearDelete(m_history);
}

void MainWindow::resize(int w, int h)
{
    ui->graphicsView->setFixedSize(w,h);
}


void MainWindow::callChoiceGameFile()
{
    m_xmlChoiceWindow = new GameFileChoice;
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
        State *newState = IA::possibleMove(*(m_currentState->info),
                                           m_game.getBoardMatrix()->get(init->y(),init->x()),
                                           m_game.getBoardMatrix()->get(final->y(),final->x()),
                                           m_game);
        if(newState)
        {
            if(m_currentState && m_currentState->next && (*newState) == (*m_currentState->next->info))//si on fait marche arrière
            {
                cancel();
            }
            else//sinon
            {
                //on recherche le coup joué après l'état courant
                List::Node<const State *> *it = m_history;
                while(it && it != m_currentState && it->next != m_currentState)
                {
                    it = it->next;
                }

                if(it && *(it->info) == (*newState))//si c'est le même que le nouveau coup
                {
                    redo();
                }
                else//si c'est un coup quelconque
                {
                    while(m_history != m_currentState)
                    {
                        List::pop_front(m_history);
                    }
                    List::push_front<const State *>(newState, m_history);
                    m_currentState = m_history;
                    m_movesNumber++;
                }
            }
            setState();
        }
    }
}

void MainWindow::cancel()
{
    if(!m_currentState)
        return;//on ne fait rien si il n'y a pas de jeu en cours
    else if(m_currentState->next)//si il y a un coup précédent
    {
        m_currentState = m_currentState->next;
        if(m_movesNumber > 0)//protection contre l'overflow !
            m_movesNumber--;
        setState();
    }
}

void MainWindow::redo()
{
    if(!m_currentState)
        return;//on ne fait rien si il n'y a pas de jeu en cours
    else if(m_history && m_currentState != m_history)
    {
        List::Node<const State *> *it = m_history;
        //on recherche le coup joué après l'état courant
        while(it && it->next != m_currentState)
        {
            it = it->next;
        }
        m_currentState = it;
        m_movesNumber++;
        setState();
    }
}

void MainWindow::showDifferentsPossibleStates()
{
    //List::Node<const State *>* possibleStates = IA::getPossibleMove(*(m_currentState->info),m_game);
    //List::Node<const State *>* IAResult = IA::aStar(m_game.getInitialState(),m_game.getFinalState(),m_game);
    //m_debugHistoricalwindow->displayGameHistory(IAResult,m_game);
    //m_debugHistoricalwindow->displayGameHistory(possibleStates,m_game);

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

