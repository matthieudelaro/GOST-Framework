#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include "mainwindow.h"
#include "list.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Gost::List::tests<int>();

    MainWindow w;

    w.show();
    return a.exec();
}
