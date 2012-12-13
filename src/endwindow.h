#ifndef ENDWINDOW_H
#define ENDWINDOW_H

#include <QWidget>
#include "mygraphicsscene.h"

/*!
 *  \brief Le namespace Ui sert à Qt Designer.
 */
namespace Ui {
class EndWindow;
}

/*!
 * \brief La classe EndWindow permet d'afficher l'état final du jeu.
 */
class EndWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EndWindow(QWidget *parent = 0);
    ~EndWindow();

    /*!
     * \brief Affiche la fin de partie de game.
     * \param game Le jeu dont on souhaite afficher la fin de partie.
     */
    void display(Game &game);

public slots:
    /*!
     *      \brief Redimensionne la fenêtre en fonction du contenu de m_scene.
     *      \param w est la largeur de m_scene
     *      \param h est la hauteur de m_scene
     */
     void resize(int w, int h);

protected:
    Ui::EndWindow *ui;///< Représente les éléments générés par QDesigner
    MyGraphicsScene *m_scene;///< Scene qui est associée à la zone graphique
};

#endif // ENDWINDOW_H
