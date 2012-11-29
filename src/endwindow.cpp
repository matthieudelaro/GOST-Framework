#include "endwindow.h"
#include "ui_endwindow.h"

EndWindow::EndWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndWindow)
{
    ui->setupUi(this);

    m_scene = new MyGraphicsScene(200);
    ui->endGraphicsView->setScene(m_scene);
    ui->endGraphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft); //permet d'aligner le (0,0) en haut et à gauche
    ui->endGraphicsView->setAcceptDrops(false);

    QObject::connect(ui->closeButton,SIGNAL(clicked()),this,SLOT(close()));
    //Appel de la fonction resize qui redimmensionne la GUI en fonction de la graphicsView
    QObject::connect(m_scene,SIGNAL(sendResize(int,int)),this,SLOT(resize(int,int)));

}

EndWindow::~EndWindow()
{
    delete ui;
    delete m_scene;
}

void EndWindow::resize(int w, int h)
{
     ui->endGraphicsView->setFixedSize(w,h);
}

void EndWindow::display(Game &game)
{
    m_scene->associateGame(&game);
    m_scene->displayMatrix();
    m_scene->addPiecesFinalState();
    //m_scene->addPiecesInitialState();
    m_scene->callResize();
}
