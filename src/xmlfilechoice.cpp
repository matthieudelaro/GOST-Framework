#include "xmlfilechoice.h"
#include "ui_xmlfilechoice.h"
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>

XmlFileChoice::XmlFileChoice(QWidget *parent) : QDialog(parent), ui(new Ui::XmlFileChoice)
{
    ui->setupUi(this);

    m_autoChoiceTableView = new QTableWidget(0,1);
    m_autoChoiceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    m_autoChoiceTableView->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    m_autoChoiceTableView->verticalHeader()->hide();
    m_autoChoiceTableView->horizontalHeader()->hide();
    m_autoChoiceTableView->setShowGrid(false);
    m_autoChoiceTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->autoChoiceLayout->addWidget(m_autoChoiceTableView);

    ui->labelChoice->setAlignment(Qt::AlignHCenter);

    QObject::connect(m_autoChoiceTableView, SIGNAL(cellClicked(int,int)),this, SLOT(openFileOfItem(int,int)));
    QObject::connect(m_autoChoiceTableView, SIGNAL(cellDoubleClicked(int,int)),this, SLOT(openFileOfItemAndQuit(int,int)));
    QObject::connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(callReturn()));
    QObject::connect(ui->buttonChoice,SIGNAL(clicked()),this,SLOT(chooseFile()));

    m_selectDir = "../res/games";
    m_files = findXMLFiles(m_selectDir);
    addFilesToTableWidget(m_files);
}

XmlFileChoice::~XmlFileChoice()
{
    delete ui;
    delete m_autoChoiceTableView;
}


void XmlFileChoice::openFileOfItem(int row, int)
{
    m_selectedPath = m_files[row];
    m_selectedFile.setFile(m_files[row]);
}

void XmlFileChoice::openFileOfItemAndQuit(int row, int)
{
    m_selectedPath = m_files[row];
    m_selectedFile.setFile(m_files[row]);
    callReturn();
}

void XmlFileChoice::chooseFile()
{
    QString fichierTmp = QFileDialog::getOpenFileName();

    //on sauvegarde le dernier resultat choisi plutôt que de remplacer le path par ""
    if(fichierTmp != "")
    {
        m_selectedFile.setFile(fichierTmp);
        m_selectedPath = fichierTmp;
    }
    ui->labelChoice->setText(m_selectedFile.fileName());
}

void XmlFileChoice::callReturn()
{
    emit returnSelectedPath(m_selectedFile.absoluteFilePath());
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

        int row = m_autoChoiceTableView->rowCount();
        m_autoChoiceTableView->insertRow(row);
        m_autoChoiceTableView->setItem(row, 0, fileNameItem);
    }
}
