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

   /* QDomDocument xml;
    QString fileName(RES_REPERTORY);
    fileName += "games/game1/xml.xml";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return EXIT_FAILURE;
    if (!xml.setContent(&file))
    {
        file.close(); // établit le document XML à partir des données du fichier (hiérarchie, etc.)
        return EXIT_FAILURE;
    }
    file.close();
*/
    QString selectDir = "../res/games";
    QStringList files = findXMLFiles(selectDir);

    qDebug()<<files;

/*
    Game game;
    if(!game.load(xml))
        return EXIT_FAILURE;
    MainWindow w;
    w.loadXMLFile(xml);
    w.show();
*/
   // return a.exec();
    return 0;
}

QStringList findXMLFiles(QString dir)
{
    QStringList filtre;
    filtre << "*.xml";
    QDirIterator dirIterator(dir, filtre ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QStringList fileList;
    while(dirIterator.hasNext()){fileList << dirIterator.next();}
    return fileList;
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
