#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileDialog>
#include "mainwindow.h"

#define RES_REPERTORY "../res/"

QStringList findXMLFiles(QString dir);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}


/**MISE DE TOUT L'INUTIL A LA SUITE POUR L'INSTANT

Matrix<int> m(1, 1, 5);
m.resize(2, 2, 6);
m(1,1) = 7;
qDebug() << m.toString();

try
{
    Game game(xml);
}
catch (std::exception &exception)
{
    qDebug() << "Erreur lors du chargement du XML.";
    qDebug() << exception.what();
}
catch (XMLFormatException &exception)
{
    qDebug() << "Erreur lors du chargement du XML.";
    qDebug() << exception.what();
}
catch (BadAllocation &exception)
{
    qDebug() << "Erreur lors du chargement du XML.";
    qDebug() << exception.what();
}
catch (int &exception)
{
    qDebug() << "Erreur lors du chargement du XML.";
    qDebug() << exception;moni
}

    if (!file.open(QIODevice::WriteOnly)) // ouverture du fichier de sauvegarde
        return EXIT_FAILURE; // en écriture
    QTextStream out(&file);
    xml.save(out, 4);
*/
