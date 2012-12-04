#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include "mainwindow.h"

#include "list.h"

void testsMatthieu()
{
    //Vector<int>::tests();
    List::tests<int>();
    //Matrix<int>::tests();
    //Graph::tests();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //testsMatthieu();

    MainWindow w;

    w.show();

    return a.exec();
}
