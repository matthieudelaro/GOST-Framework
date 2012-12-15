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
 *  \brief Le namespace Ui sert à Qt Designer.
 */
namespace Ui {
class MainWindow;
}

/*!
 *      \brief La classe MainWindow gère le GUI du projet.
 *      Elle permet de charger un fichier xml ou texte via une instance de Game, de l'associer et de l'afficher à travers la m_scene.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);///< Le contructeur.

    /*!
     *      \brief Charge un jeu.
     *      \param path Le nom du fichier à charger.
     *      \param error Si error est différent de NULL, alors error est utilisé pour renvoyer les erreurs de lecture du fichier xml
     *      \return true si le chargement a été effectué avec succès, false sinon.
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
     *  \brief Recherche et affiche la solution.
     */
    void researchSolution();

    /*!
     *  \brief Permet au joueur de recommencer une partie.
     */
    void newGame();

    /*!
     *  \brief montre les différents états possibles
     */
    void showDifferentsPossibleStates();

protected:
    GameFileChoice *m_xmlChoiceWindow;///< Fenetre de choix du fichier Xml
    Ui::MainWindow *ui;///< Représente les éléments générés par QDesigner
    MyGraphicsScene *m_scene;///< Scene qui est associée à la zone graphique
    Game m_game;///< Game qui contient tout le jeu
    QString m_loadedPath;///< Chemin du Xml chargé
    /*!
     * \brief Représente l'état final du jeu.
     *
     * \warning Ce n'est pas un état valide si on considère les pointeurs vers les morceaux de cases. car on y retrouve plusieurs cases
     * contenant des pointeurs vers les même morceaux de pièces. Par contre, les info de ces pointeurs contiennent des
     * numéros de pièces valides, avec 0 signifiant "Jocker"
     */
    EndWindow *m_finalStateWindow;
    HistoricalWindow *m_historicalWindow;///< La fenêtre affichant l'historique du jeu.
    HistoricalWindow *m_solutionWindow;///< La fenêtre affichant la solution du jeu.
    HistoricalWindow *m_debugHistoricalwindow;///< La fenêtre de debug.
    Gost::List::Node<const Gost::State *> *m_currentState;///< L'état courant dans l'historique.
    Gost::List::Node<const Gost::State *> *m_history;///< Liste contenant l'historique des mouvements de l'utilisateur.
    unsigned int m_movesNumber;///< Le nombre de coups effectués par le joueur.
    QLabel *m_labelMovesNumber;///< Le label affichant le nombre de coups effectués par le joueur. Vide si celui-ci vaut 0.
};

#endif // MAINWINDOW_H
