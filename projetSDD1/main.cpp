#include <QtGui/QApplication>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"

#define RES_REPERTORY "../res/"

#include "game.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QDomDocument xml;
    QString fileName(RES_REPERTORY);
    fileName += "games/leNomNAAucuneImportance/xml2.xml";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return EXIT_FAILURE;
    if (!xml.setContent(&file))
    {
        file.close(); // établit le document XML à partir des données du fichier (hiérarchie, etc.)
        return EXIT_FAILURE;
    }
    file.close();
/*
    Matrix<int> m(1, 1, 5);
    m.resize(2, 6, 6);
    m(1,1) = 7;
    qDebug() << m.toString();
*/
/*    try
    {
        Game game(xml);
    }
    catch (std::exception &exception)
    {
        qDebug() << "Erreur lors du chargement du XML.";
        qDebug() << exception.what();
    }
    /*catch (XMLFormatException &exception)
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
    }*/

/*    if (!file.open(QIODevice::WriteOnly)) // ouverture du fichier de sauvegarde
        return EXIT_FAILURE; // en écriture
    QTextStream out(&file);
    xml.save(out, 4);
*/
    Game game(xml);

    MainWindow w(game);
    w.show();

    return a.exec();
}
