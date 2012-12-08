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

class HistoricalWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HistoricalWindow(QWidget *parent = 0);
    void displayGameHistory(List::Node<const State *> *possibleStates, Game &game, bool oppositeOrder = false);
    ~HistoricalWindow();

public slots:
    void resize(int w, int h);

private:
    void addStates(List::Node<const State *> *possibleStates, Game &game, bool oppositeOrder = false);
    Ui::HistoricalWindow *ui;
    List::Node<MyGraphicsScene*> *m_scenes;
    List::Node<QGraphicsView*> *m_views;
    QVBoxLayout *m_verticalLayout;
};

#endif // HISTORICALWINDOW_H
