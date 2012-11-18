#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include "mainwindow.h"

#include "list.h"
void testsMatthieu()
{
    Vector<Graph::Node> v;
    v.resize(3);

    List::Node<int>* l = NULL;
    List::push_front(1, l);
    List::push_front(2, l);
    List::push_front(3, l);

    qDebug() << "l :";
    List::Node<int>* it = l;
    while(it)
    {
        qDebug() << it->info;
        it = it->next;
    }
    qDebug() << "size(l) = " << List::size(l);

    List::clear(l);
    qDebug() << "size(l) = " << List::size(l);
    qDebug() << "l :";
    it = l;
    while(it)
    {
        qDebug() << it->info;
        it = it->next;
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    testsMatthieu();

    MainWindow w;

    w.show();

    return a.exec();
}
