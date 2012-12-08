#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include "mainwindow.h"

#include "list.h"

void testsMatthieu()
{
    //Vector<int>::tests();
    //List::tests<int>();
    //Matrix<int>::tests();
    //Graph::tests();

   /* QString file;
    file += "ABCX\n";
    file += "....\n";
    file += "\n";
    file += "..#X\n";
    file += ".ABC\n";

    Game game;
    game.load(file);*/



}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    testsMatthieu();

    MainWindow w;

    w.show();
    return a.exec();
}
