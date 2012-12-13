#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    w.show();
    return a.exec();
}
