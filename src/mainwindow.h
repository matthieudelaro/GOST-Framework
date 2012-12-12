#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QDomDocument>
#include <QLabel>
#include "mygraphicsscene.h"
#include "xmlfilechoice.h"
#include "game.h"
#include <QMessageBox>
#include "endwindow.h"
#include "historicalwindow.h"
#include "list.h"
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
    bool loadGameFromPath(QString &path, QString *error = NULL);

    /*!
     *      \brief Met à jour le jeu avec m_currentState->info
     */
    void setState();

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
    void callChoiceGameFile();

    /*!
     *      \brief saveSelectedPathFromXml sauvegarde le chemin du fichier xml choisis
     *      \param path est le chemin du xml choisi
     */
    void saveSelectedPathFromXml(QString path);

    /*!
     *  \brief determine la direction et la pièce appelée
     */
    void callIAPossibleMove(QPointF *init, QPointF *final);

    /*!
     *  \brief Annule le dernier coup joué par l'utilisateur.
     */
    void cancel();

    /*!
     *  \brief Implémente Ctrl+Y. Permet de rejouer un coup qu'on avait annulé.
     *  \return true si on peut revenir dans le temps, false sinon.
     */
    void redo();

    /*!
     *  \brief montre les différents états possibles
     */
    void showDifferentsPossibleStates();

private:
    XmlFileChoice *m_xmlChoiceWindow;///< Fenetre de choix du fichier Xml
    Ui::MainWindow *ui;///< Fenetre principale, conenant le GUI
    MyGraphicsScene *m_scene;///< Scene qui est associée à la zone graphique
    Game m_game;///< Game qui contient tout le jeu
    QString m_loadedPath;///< Chemin du Xml chargé
    EndWindow *m_finalStateWindow;
    HistoricalWindow *m_historicalWindow;
    HistoricalWindow *m_debugHistoricalwindow;
    //const State *m_currentState;
    List::Node<const State *> *m_currentState;
    List::Node<const State *> *m_history;///< Liste contenant l'historique des mouvements de l'utilisateur.
    unsigned int m_movesNumber;
    QLabel *m_labelMovesNumber;
};

#endif // MAINWINDOW_H
