#include "xmlfilechoice.h"
#include "ui_xmlfilechoice.h"
#include <QDebug>
#include <QDirIterator>

XmlFileChoice::XmlFileChoice(QWidget *parent) : QDialog(parent), ui(new Ui::XmlFileChoice)
{
    ui->setupUi(this);

    autoChoiceTableView = new QTableWidget(0,1);
    autoChoiceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    autoChoiceTableView->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    autoChoiceTableView->verticalHeader()->hide();
    autoChoiceTableView->horizontalHeader()->hide();
    autoChoiceTableView->setShowGrid(false);
    autoChoiceTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->autoChoiceLayout->addWidget(autoChoiceTableView);

    QObject::connect(autoChoiceTableView, SIGNAL(cellClicked(int,int)),this, SLOT(openFileOfItem(int,int)));
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(callReturn()));

    selectDir = "../res/games";
    files = findXMLFiles(selectDir);
    addFilesToTableWidget(files);
}

XmlFileChoice::~XmlFileChoice()
{
    delete ui;
    delete autoChoiceTableView;
}


void XmlFileChoice::openFileOfItem(int row, int)
{
    selectedPath = files[row];
}

void XmlFileChoice::callReturn()
{
    emit returnSelectedPath(selectedPath);
}

QStringList XmlFileChoice::findXMLFiles(QString dir)
{
    QStringList filtre;
    filtre << "*.xml";
    QDirIterator dirIterator(dir, filtre ,QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QStringList fileList;
    while(dirIterator.hasNext()){fileList << dirIterator.next();}
    return fileList;
}

void XmlFileChoice::addFilesToTableWidget(QStringList &files)
{
    for (int i = 0; i < files.size(); i++)
    {
        QTableWidgetItem *fileNameItem = new QTableWidgetItem(files[i]);

        int row = autoChoiceTableView->rowCount();
        autoChoiceTableView->insertRow(row);
        autoChoiceTableView->setItem(row, 0, fileNameItem);
    }
}
