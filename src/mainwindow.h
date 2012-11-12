#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QDomDocument>
#include "mygraphicsscene.h"
#include "game.h"

/*!
    \brief La classe MainWindow gère le GUI du projet.
    Elle permet de charger un fichier XML, de l'associer et de l'afficher à travers la m_scene.
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
        \brief loadXMLFile récupère un document XML et génère m_game avec celui ci
        \param xml est un document qui contient le XML
        */
    void loadXMLFile(QDomDocument &xml);
    ~MainWindow();

public slots:
    /*!
      \brief resize redimentionne la fenetre en fonction du contenu de m_scene
      \param w est la largeur de m_scene
      \param h est la hauteur de m_scene
      */
    void resize(int w, int h);

    /*!
      \brief callAssociationMatrix associe la matrice de m_game à m_scene
      */
    void callAssociateMatrix();

    /*!
      \brief callDisplay fait un rendu graphique de la matrice de m_game à travers m_scene
      */
    void callDisplay();

private:
    /*!

      */
    Ui::MainWindow *ui;
    MyGraphicsScene *m_scene;
    Game m_game;
};

#endif // MAINWINDOW_H
