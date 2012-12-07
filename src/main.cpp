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


    QFile file("../res/games/aneRouge.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        qDebug() << "fichier bien ouvert en lecture seule";
    }
    qDebug() << file.size();
    QTextStream textStream(&file);
    qDebug() << textStream.readAll();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //testsMatthieu();

    MainWindow w;

    w.show();

    return a.exec();
}
