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
    void displayGameHistory(List::Node<const State *> *possibleStates, Game &game);
    ~HistoricalWindow();

public slots:
    void resize(int w, int h);

private:
    Ui::HistoricalWindow *ui;
    List::Node<MyGraphicsScene*> *m_scenes;
    List::Node<QGraphicsView*> *m_views;
    QVBoxLayout *m_verticalLayout;
};

#endif // HISTORICALWINDOW_H
