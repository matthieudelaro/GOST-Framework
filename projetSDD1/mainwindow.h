#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include "game.h"
#include "matrix.h"
#include "mygraphicsscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setGame(Game& g);
    ~MainWindow();

public slots:
    void resize(int w, int h);  //redimensionne le graphicsView en fonction de son contenu
    void callSetMatrix();       //appelle l'utilisation de la matrice pour afficher le plateau

private:
    Ui::MainWindow *ui;
    MyGraphicsScene *myScene;
    Matrix<Graph::Node*> toView;
};

#endif // MAINWINDOW_H
