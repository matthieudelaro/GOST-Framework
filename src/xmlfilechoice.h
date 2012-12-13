#ifndef XMLFILECHOICE_H
#define XMLFILECHOICE_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QStringList>
#include <QFileInfo>

namespace Ui {class XmlFileChoice;}

class XmlFileChoice : public QDialog
{
    Q_OBJECT

public:
    explicit XmlFileChoice(QWidget *parent = 0);
    QStringList findXMLFiles(QString dir);
    void addFilesToTableWidget(QStringList &m_files);
    ~XmlFileChoice();

public slots:
    void openFileOfItem(int, int);
    void openFileOfItemAndQuit(int row, int);
    void chooseFile();
    void callReturn();

signals:
    void returnSelectedPath(QString);

protected:
    Ui::XmlFileChoice *ui;
    QTableWidget *m_autoChoiceTableView;
    QString m_selectDir;
    QStringList m_files;
    QString m_selectedPath;
    QFileInfo m_selectedFile;
};

#endif // XMLFILECHOICE_H
