#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QDomDocument>
#include "mygraphicsscene.h"
#include "xmlfilechoice.h"
#include "game.h"
#include "endwindow.h"
#include "IA.h"

    /*!
     *      \brief La classe MainWindow gère le GUI du projet.
     *      Elle permet de charger un fichier XML, de l'associer et de l'afficher à travers la m_scene.
     */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    /*!
     *      \brief loadGameFromXml va charger le document Xml dans la classe game et afficher le jeu
     *      \param xml est un document qui contient le Xml.
     */
    bool loadGameFromXml(QDomDocument &xml);

    /*!
     *      \brief loadXmlFromPath va tenter de charger le document Xml en fonction du path demandé
     *      \param path est le chemin du document Xml à charger
     *      \return Un message d'erreur si la fonction de chargement a échouée
     */
    int loadXmlFromPath(QString path);

    /*!
     *      \brief destructeur de la classe
     */
    ~MainWindow();

public slots:
    /*!
     *      \brief resize redimensionne la fenêtre en fonction du contenu de m_scene.
     *      \param w est la largeur de m_scene
     *      \param h est la hauteur de m_scene
     */
    void resize(int w, int h);


    /*!
     *      \brief callChoiceXmlFile appelle la fenêtre de choix de fichier XML
     */
    void callChoiceXmlFile();

    /*!
     *      \brief saveSelectedPathFromXml sauvegarde le chemin du fichier xml choisis
     *      \param path est le chemin du xml choisi
     */
    void saveSelectedPathFromXml(QString path);

    /*!
     *      \brief callLoadGameFromXml va appeler la fonction loadGameFromXml
     */
    void callLoadGameFromXml();

    /*!
     *  \brief determine la direction et la pièce appelée
     */
    void findPositionAndPiece(QPointF *init, QPointF *final);

private:
    XmlFileChoice *m_xmlChoiceWindow;///< Fenetre de choix du fichier Xml
    Ui::MainWindow *ui;///< Fenetre principale, conenant le GUI
    MyGraphicsScene *m_scene;///< Scene qui est associée à la zone graphique
    Game m_game;///< Game qui contient tout le jeu
    QString m_loadedPath;///< Chemin du Xml chargé
    QDomDocument m_XMLFileChosed;///< Document qui récupère le document Xml choisi
    EndWindow *finalStateWindows;
};

#endif // MAINWINDOW_H
