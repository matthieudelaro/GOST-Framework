#ifndef XMLFILECHOICE_H
#define XMLFILECHOICE_H

#include <QDialog>
#include <QTableWidget>
#include <QString>
#include <QStringList>
#include <QFileInfo>

namespace Ui {class GameFileChoice;}

/*!
 * \brief La classe GameFileChoice permet à l'utilisateur de sélectionner un jeux
 */
class GameFileChoice : public QDialog
{
    Q_OBJECT

public:
    explicit GameFileChoice(QWidget *parent = 0);///< Le contructeur.

    /*!
     * \brief Recherche tous les fichiers xml dans res/games/
     * \return La liste des noms des fichiers.
     */
    QStringList findXMLFiles();

    /*!
     * \brief Affiche une liste de noms de fichiers dans la liste visualisée par l'utilisateur.
     * \param files La liste des noms de fichiers à afficher.
     */
    void addFilesToTableWidget(QStringList &files);

    /*!
     *  \brief Libère la mémoire.
     */
    ~GameFileChoice();

public slots:
    void openFileOfItem(int, int);///< Réimplémentation de la méthode de QDialog.
    void openFileOfItemAndQuit(int row, int);///< Réimplémentation de la méthode de QDialog.
    void chooseFile();///< Propose à l'utilisateur de charger un jeu qui n'aurait pas été trouvé automatiquement (jeu au format Pepper par exemple)
    void callReturn();///< Renvoie à la MainWindow le chemin du fichier sélectionné par l'utilisateur.

signals:
    void returnSelectedPath(QString);///< Signal permettant de à la MainWindow le chemin du fichier sélectionné par l'utilisateur.

protected:
    Ui::GameFileChoice *ui;///< Représente les éléments générés par QDesigner
    QTableWidget *m_autoChoiceTableView;///< La tableau dans lequel sont rangés les fichiers trouvés automatiquement.
    QStringList m_files;///< La liste des fichiers.
    QString m_selectedPath;///< Le chemin d'accès au fichier choisi.
    QFileInfo m_selectedFile;///< Le fichier choisi.
};

#endif // XMLFILECHOICE_H
