#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QDomDocument>
#include "mygraphicsscene.h"
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void loadXMLFile(QDomDocument &xml);
    ~MainWindow();

public slots:
    void resize(int w, int h);

private:
    Ui::MainWindow *ui;
    MyGraphicsScene *m_scene;
    Game m_game;
};

#endif // MAINWINDOW_H
