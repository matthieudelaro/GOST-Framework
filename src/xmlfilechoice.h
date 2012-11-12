#ifndef XMLFILECHOICE_H
#define XMLFILECHOICE_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QStringList>

namespace Ui {class XmlFileChoice;}

class XmlFileChoice : public QDialog
{
    Q_OBJECT

public:
    explicit XmlFileChoice(QWidget *parent = 0);
    QStringList findXMLFiles(QString dir);
    void addFilesToTableWidget(QStringList &files);
    ~XmlFileChoice();

public slots:
    void openFileOfItem(int, int);
    void callReturn();

signals:
    void returnSelectedPath(QString);

private:
    Ui::XmlFileChoice *ui;
    QTableWidget *autoChoiceTableView;
    QString selectDir;
    QStringList files;
    QString selectedPath;
};

#endif // XMLFILECHOICE_H
