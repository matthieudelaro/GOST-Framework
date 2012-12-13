#ifndef HISTORICALWINDOW_H
#define HISTORICALWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QVBoxLayout>
#include "vector.h"
#include "mygraphicsscene.h"
#include "list.h"

namespace Ui {
class HistoricalWindow;
}

/*!
 * \brief La classe HistoricalWindow permet d'afficher une suite d'état du jeu.
 *
 *  On l'utilise principalement pour permettre à l'utilisateur de visualiser l'historique de ses coups,
 *  mais elle est aussi très pratique pour afficher n'importe quelle liste d'états, comme les coups suivant
 *  possibles, les listes générées par nos algorithmes de recherche, ...
 */
class HistoricalWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoricalWindow(QWidget *parent = 0);
    /*!
     * \brief displayGameHistory permet de changer la liste d'états que la fenêtre affiche.
     * \param possibleStates La liste des nouveaux états à afficher.
     * \param game Le jeu auquel correspondent les états.
     * \param oppositeOrder Permet d'inverser l'ordre d'affichage des éléments de la liste d'états.
     */
    void displayGameHistory(List::Node<const State *> *possibleStates, Game &game, bool oppositeOrder = false);
    ~HistoricalWindow();

public slots:
    void resize(int w, int h);

private:
    /*!
     * \brief addStates est appelée récursivement par displayGameHistory. Elle affiche les éléments de la liste un à un,
     * en changeant l'ordre si besoin est en fonction de oppositeOrder.
     * \param possibleStates La liste des nouveaux états à afficher.
     * \param game Le jeu auquel correspondent les états.
     * \param oppositeOrder Permet d'inverser l'ordre d'affichage des éléments de la liste d'états.
     */
    void addStates(List::Node<const State *> *possibleStates, Game &game, bool oppositeOrder = false);
    Ui::HistoricalWindow *ui;///< Représente les éléments générés par QDesigner
    List::Node<MyGraphicsScene*> *m_scenes;///< Contient les représentations graphiques des états.
    List::Node<QGraphicsView*> *m_views;///< Contient les représentations graphiques des états.
    QVBoxLayout *m_verticalLayout;///< Permet d'afficher les états les uns au dessus des autres.
};

#endif // HISTORICALWINDOW_H
