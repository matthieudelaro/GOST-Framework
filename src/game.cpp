#include "game.h"
#include "tools.h"
#include <QStringList>

Game::Game(const Game &original)
{
    throw "Appel du constructeur de copie de Game !";
}

Game &Game::Game::operator =(const Game &original)
{
    throw "Appel de l'opérateur d'affectation de Game !";
}

Game::Game()
{
    m_board = NULL;
    m_jocker = NULL;
    m_pieces = NULL;
    m_nbNodes = 0;
}

bool Game::load(QDomDocument &xml, QString *error)
{
    //on prépare la sortie d'erreur
    QString* log;
    if(error)
        log = error;
    else
        log = new QString();


    clear();//on vide le jeu, au cas où un autre serait en mémoire

    //QDomElement root = xml.documentElement();
    QDomNodeList tempListBoard = xml.elementsByTagName("board");
    QDomNodeList tempListInitialShape = xml.elementsByTagName("initialShape");
    QDomNodeList tempListFinalShape = xml.elementsByTagName("finalShape");
    if(!tempListBoard.isEmpty() && !tempListInitialShape.isEmpty() && !tempListFinalShape.isEmpty())
    {
        QDomNode board = tempListBoard.item(0);
        QDomElement initialShape = tempListInitialShape.item(0).toElement();
        QDomElement finalShape = tempListFinalShape.item(0).toElement();
        if(initialShape.attribute("type") == "defined") //pour le mode aléatoire, voir le Knuth's Shuffle
        {
            QDomNodeList tempListcolumns = xml.elementsByTagName("column");
            //QDomNodeList listLines = xml.elementsByTagName("line");
            QDomNodeList listLines = initialShape.childNodes();
            QDomNodeList listFinalLines = finalShape.childNodes();
            if(!tempListcolumns.isEmpty() && !listLines.isEmpty() && !listFinalLines.isEmpty())
            {
                //Pour chaque pièce : son numéro, un tableau 2D de Graph::Node*, un Graph::Node* pour le début de la pièce
                List::Node<Triple<int, Matrix<Graph::Node*>, Graph::Node*> > *pieces = NULL;


                // Etape 1 :
                //On calcule le nombre de lignes
                //On calcule le nombre de colonnes de la ligne la plus longue
                //On calcule le nombre de nodes
                //On repère les nombres des différentes pièces
                unsigned int nbLines = ((unsigned int)listLines.count()), nbMaxColumns = 0;
                m_nbNodes = 0;
                m_board = NULL;

                if(((unsigned int)listFinalLines.count()) != nbLines)
                {
                    *log += QString("Il n'y a pas autant de lignes dans l'état final que dans l'état initial.");
                    clear();
                    return false;
                }

                for(unsigned int line = 0; line < nbLines; ++line)
                {
                    QDomNodeList columns = listLines.item(line).childNodes();
                    QDomNodeList finalColumns = listFinalLines.item(line).childNodes();

                    unsigned int nbColumns = ((unsigned int)columns.count());
                    if(((unsigned int)finalColumns.count()) != nbColumns)
                    {
                        //*log += "Il n'y a pas autant de columns dans l'état final (" + QString(finalColumns.count()) + ") que dans l'état initial (" + QString(nbColumns) + ") pour la line " + QString(line) + "\n";
                        *log += QString("Il n'y a pas autant de colonnes dans l'état final (%1) que dans l'état initial (%2) pour la ligne %3.\n").arg(finalColumns.count()).arg(nbColumns).arg(line);
                        clear();
                        return false;
                    }

                    if(nbColumns > nbMaxColumns)
                        nbMaxColumns = nbColumns;
                    for(unsigned int column = 0; column < nbColumns; ++column)
                    {
                        QDomElement element = columns.item(column).toElement();
                        QDomElement finalElement = finalColumns.item(column).toElement();
                        if(element.attribute("type") != "void")
                        {
                            if(finalElement.attribute("type") == "void")
                            {
                                //*log += "Il n'y a pas autant de columns dans l'état final que dans l'état initial pour la line " + QString(line) + "\n";
                                //*log += QString("Il n'y a pas autant de colonne dans l'état final que dans l'état initial pour la line %1.\n").arg(line);
                                *log += QString("Incohérence entre l'état initial et l'état final (ligne = %1), colonne = %2) : est-ce que la case existe ? \n").arg(line).arg(column);
                                clear();
                                return false;
                            }
                            else if(finalElement.attribute("type") == "piece" && finalElement.attribute("number").toInt() == 0)
                            {
                                //*log += "Le numéro de pièce 0 est réservé pour le jocker (finalState, ligne = " + QString(line) + ", colonne = " + QString(column) + ")" + "\n";
                                *log += QString("Le numéro de pièce 0 est réservé pour le jocker (finalState, ligne = %1, colonne = %2) \n").arg(line).arg(column);
                                clear();
                                return false;
                            }

                            m_nbNodes++;
                            if(element.attribute("type") == "piece")
                            {
                                if(element.attribute("number").toInt() == 0)
                                {
                                    *log += QString("Le numéro de pièce 0 est réservé pour le jocker (initialState, ligne = %1, colonne = %2) \n").arg(line).arg(column);
                                    clear();
                                    return false;
                                }

                                if(!List::find(element.attribute("number").toInt(), pieces))
                                {
                                    Triple<int, Matrix<Graph::Node*>, Graph::Node*> piece;
                                    piece.first = element.attribute("number").toInt();
                                    piece.third = NULL;
                                    List::push_front(piece, pieces);
                                }
                            }
                        }
                        else if(!(finalElement.attribute("type") == "void"))
                        {
                            *log += QString("Incohérence entre l'état initial et l'état final (ligne = %1), colonne = %2) : est-ce que la case existe ? \n").arg(line).arg(column);
                            clear();
                            return false;
                        }
                    }
                }

                //Avec toutes les informations que l'on vient de récolter avec ce premier passage,
                //on initialise m_index, m_initialState, m_finalState, m_boardMatrix, et les matrices
                //temporaires de chaque pièce.
                m_index.resize(m_nbNodes, NULL);
                m_initialState.resize(m_nbNodes, NULL);
                m_finalState.resize(m_nbNodes, NULL);
                m_boardMatrix.resize(nbLines, nbMaxColumns, NULL);

                List::Node<Triple<int, Matrix<Graph::Node*>, Graph::Node*> > *it = pieces;

                while(it)
                {
                    it->info.second.resize(nbLines, nbMaxColumns, NULL);
                    it = it->next;
                }

                //Etape 2 :
                //Parcours du XML
                //Remplissage des matrices
                unsigned int index = 0;
                for(unsigned int line = 0; line < nbLines; ++line)
                {
                    QDomNodeList columns = listLines.item(line).childNodes();
                    unsigned int nbColumns = ((unsigned int)columns.count());
                    for(unsigned int column = 0; column < nbColumns; ++column)
                    {
                        it = pieces;

                        QDomElement element = columns.item(column).toElement();
                        //finalement il est inutile d'initialiser à NULL, vu que c'est déjà fait
                        //à la fin de l'étape 1.
                        /*if(element.attribute("type") == "void")
                        {
                            m_boardMatrix(line, column) = NULL;
                            while(it)
                            {
                                it->info.second(line, column) = NULL;
                                it = it->next;
                            }
                        }
                        else*/
                        if(element.attribute("type") != "void")
                        {
                            m_boardMatrix(line, column) = new Graph::Node(index);
                            if(!m_boardMatrix(line, column))
                                throw BadAllocation("Impossible to allocate a new Node for m_boardMatrix.");

                            if(m_board == NULL)
                                m_board = m_boardMatrix(line, column);
                            m_index[index] = m_boardMatrix(line, column);
                            if(element.attribute("type") == "piece")
                            {
                                while(it)
                                {
                                    if(element.attribute("number").toInt() == it->info.first)
                                    {
                                        //on crée un noeud dont la valeur correspond au numéro de la pièce
                                        it->info.second(line, column) = new Graph::Node(it->info.first);

                                        m_initialState[index] = it->info.second(line, column);

                                        //si c'est la première fois qu'on tombe sur un noeud de cette pièce
                                        if(it->info.third == NULL)
                                            it->info.third = it->info.second(line, column);
                                    }
                                    else
                                    {
                                        it->info.second(line, column) = NULL;
                                    }
                                    it = it->next;
                                }
                            }
                            index++;
                        }
                    }
                }

                //Etape 3 :
                //On relie les Graph::Node* de chaque case de chaque Matrix si besoin
                //On initialise m_pieces avec le début de chaque pièce
                for(unsigned int line = 0; line < m_boardMatrix.getHeight(); ++line)
                {
                    for(unsigned int column = 0; column < m_boardMatrix.getWidth(); ++column)
                    {
                        if(m_boardMatrix(line, column)) //si il y a quelque chose à cet emplacement
                        {
                            //On lit avec les Node adjacents.
                            if(line > 0)
                            {
                                (*(m_boardMatrix(line, column)))[0] = m_boardMatrix(line-1, column);
                            }
                            if(line + 1 < m_boardMatrix.getHeight())
                                (*(m_boardMatrix(line, column)))[1] = m_boardMatrix(line+1, column);
                            if(column > 0)
                                (*(m_boardMatrix(line, column)))[2] = m_boardMatrix(line, column-1);
                            if(column + 1 < m_boardMatrix.getWidth())
                                (*(m_boardMatrix(line, column)))[3] = m_boardMatrix(line, column+1);

                            //Pour avoir quelque chose de plus générique :
                            //for(unsigned int link = 0; link < Graph::nbLinks; ++link)
                            //    m_boardMatrix(line, column)[link] = ?
                            //=> inutile de toute façon puisque le format paramétrage des jeux n'est adapté qu'à la 2D
                        }

                        //on fait la même chose pour chaque pièce :
                        it = pieces;
                        while(it)
                        {
                            if(it->info.second(line, column))// si la case (line, column) existe
                            {
                                if(line > 0)//si line-1 existe
                                    (*(it->info.second(line, column)))[0] = it->info.second(line-1, column);
                                if(line + 1 < it->info.second.getHeight())//si line+1 existe
                                    (*(it->info.second(line, column)))[1] = it->info.second(line+1, column);
                                if(column > 0)//si columne-1 existe
                                    (*(it->info.second(line, column)))[2] = it->info.second(line, column-1);
                                if(column + 1 < it->info.second.getWidth())//si column+1 existe
                                    (*(it->info.second(line, column)))[3] = it->info.second(line, column+1);
                            }
                            it = it->next;
                        }
                    }
                }

                //Etape 4 :
                //on initialise l'état final
                m_jocker = new Graph::Node(0);
                index = 0;
                for(unsigned int line = 0; line < nbLines; ++line)
                {
                    QDomNodeList finalColumns = listFinalLines.item(line).childNodes();
                    unsigned int nbColumns = ((unsigned int)finalColumns.count());
                    for(unsigned int column = 0; column < nbColumns; ++column)
                    {
                        if(m_boardMatrix.getConst(line, column))
                        {
                            QDomElement finalElement = finalColumns.item(column).toElement();
                            if(finalElement.attribute("type") == "piece")
                            {
                                List::Node<Triple<int, Matrix<Graph::Node*>, Graph::Node*> > *piece = List::find(finalElement.attribute("number").toInt(), pieces);
                                if(piece)
                                {
                                    m_finalState.get(index) = piece->info.third;
                                }
                                else
                                {
                                    //*log += "Piece dans finalState inexistante dans initialState (pièce numéro " + QString(finalElement.attribute("number").toInt()) + ")\n";
                                    *log += QString("Piece dans finalState inexistante dans initialState (pièce numéro %1).").arg(finalElement.attribute("number").toInt());
                                    clear();
                                    return false;
                                }
                                /*List::Node<Graph::Node*> *it = m_pieces;
                                bool found = false;
                                while(it && !found)
                                {
                                    if(it->info->info == (unsigned int) finalElement.attribute("number").toInt())
                                        found = true;
                                    else
                                        it = it->next;
                                }
                                if(found)
                                {
                                    m_finalState.get(index) = it->info;
                                }
                                else
                                {
                                    *log += "Piece dans finalState inexistante dans initialState (pièce numéro " << finalElement.attribute("number").toInt() << ")";
                                    clear();
                                    return false;
                                }*/
                            }
                            else if(finalElement.attribute("type") == "jocker")
                            {
                                m_finalState.get(index) = m_jocker;
                            }
                            index++;
                        }
                    }
                }

                //On initialise m_pieces
                m_pieces = NULL;
                it = pieces;
                while(it)
                {
                    List::push_front(it->info.third, m_pieces);
                    it = it->next;
                }

                //on libère pieces
                List::clear(pieces);
            }
            else
            {
                *log += QString("Nodes initialShape, finalShape, et/ou board manquant(s).\n");
                clear();
                return false;
            }
        }
        else
        {
            *log += QString("Nodes initialShape, finalShape, et/ou board manquant(s).\n");
            clear();
            return false;
        }
    }
    else
    {
        *log += QString("Nodes initialShape, finalShape, et/ou board manquant(s).\n");
        clear();
        return false;
    }
    return true;
}

/*
 *  Cette fonction convertit les fichiers du format Pepper au format xml, puis charge le xml ainsi créé xml.
 */
bool Game::load(const QString &file, QString* error)
{
    //on prépare la sortie d'erreur
    QString* log;
    if(error)
        log = error;
    else
        log = new QString();


    *log += "Début de Game::load(QString)";
    QChar empty = '.', nonexistent = 'X', jocker = '#';
    QStringList lines = file.split("\n");//on split le fichier en lignes, en concervant les lignes vides

    //on initialise un xml basique
    QString basicXml;
    basicXml += "<?xml version='1.0' encoding='UTF-8'?>";
    basicXml += "<game>";
    basicXml += "   <board>";
    basicXml += "       <initialShape type=\"defined\">";
    basicXml += "       </initialShape>";
    basicXml += "       <finalShape>";
    basicXml += "       </finalShape>";
    basicXml += "   </board>";
    basicXml += "</game>";
    QDomDocument xml;
    xml.setContent(basicXml);

    QDomNodeList tempListInitialShape = xml.elementsByTagName("initialShape");
    QDomElement initialShape = tempListInitialShape.item(0).toElement();

    QDomNodeList tempListFinalShape = xml.elementsByTagName("finalShape");
    QDomElement finalShape = tempListFinalShape.item(0).toElement();

    //On lit le fichier :
    //pour chaque ligne de l'état initial
    QStringList::const_iterator line;
    for(line = lines.constBegin(); line != lines.constEnd() && !(*line).isEmpty(); ++line)
    {
        QDomElement lineElement = xml.createElement("line");
        initialShape.appendChild(lineElement);

        //pour chaque élément de la ligne
        for(int column = 0; column < (*line).length(); ++column)
        {
            QDomElement columnElement = xml.createElement("column");
            if((*line)[column] == empty)
            {
                columnElement.setAttribute("type", "free");
            }
            else if((*line)[column] == nonexistent)
            {
                columnElement.setAttribute("type", "void");
            }
            //else if(('A' <= (*line)[column] && (*line)[column] <= 'Z') || //si c'est une lettre majuscule
            //        ('z' <= (*line)[column] && (*line)[column] <= 'z'))//si c'est une lettre minucule
            else if((*line)[column].isLetter())
            {
                columnElement.setAttribute("type", "piece");
                columnElement.setAttribute("number", QString("%1").arg(int((*line)[column].toAscii() + 1 - QChar('A').toAscii())));
            }
            else
            {
                *log += "Caractère inconnu.";
                return false;
            }
            lineElement.appendChild(columnElement);
        }
    }

    //pour chaque ligne de l'état final
    ++line;
    for(; line != lines.constEnd() && !(*line).isEmpty(); ++line)
    {
        QDomElement lineElement = xml.createElement("line");
        finalShape.appendChild(lineElement);

        //pour chaque élément de la ligne
        for(int column = 0; column < (*line).length(); ++column)
        {
            QDomElement columnElement = xml.createElement("column");
            if((*line)[column] == empty)
            {
                columnElement.setAttribute("type", "free");
            }
            else if((*line)[column] == nonexistent)
            {
                columnElement.setAttribute("type", "void");
            }
            else if((*line)[column] == jocker)
            {
                columnElement.setAttribute("type", "jocker");
            }
            //else if(('A' <= (*line)[column] && (*line)[column] <= 'Z') || //si c'est une lettre majuscule
            //        ('z' <= (*line)[column] && (*line)[column] <= 'z'))//si c'est une lettre minucule
            else if((*line)[column].isLetter())
            {
                columnElement.setAttribute("type", "piece");
                columnElement.setAttribute("number", QString("%1").arg(int((*line)[column].toAscii() + 1 - QChar('A').toAscii())));
            }
            else
            {
                *log += "Caractère inconnu.";
                return false;
            }
            lineElement.appendChild(columnElement);
        }
    }
    *log += "Résultat :";
    *log += xml.toString();
    *log += "Fin de Game::load(QString)";

    if(!error)
        qDebug() << *log;
    return load(xml, log);
}

const Graph::Node *Game::getBoardNode(const Graph::Node *&pieceNode, const State &state) const
{
    for(unsigned int i = 0; i < state.getLength(); ++i)
        if(state[i] == pieceNode)
            return m_index[i];

    //si on n'a pas trouvé
    return NULL;
}

bool Game::getNodePieceIndex(const Graph::Node* &pieceNode, unsigned int &index, const State &state) const
{
    for(index = 0; index < state.getLength(); ++index)
        if(state[index] == pieceNode)
            return true;

    //si on n'a pas trouvé la pièce
    return false;
}

const Graph::Node *Game::getPieceNode(unsigned int line, unsigned int column, const State &state) const
{
    if(m_boardMatrix(line,column) == NULL)
        return NULL;
    else
        return state[m_boardMatrix(line, column)->info];
}

void Game::clear()
{
    //Supprimer le m_board
    Graph::clear(m_board);

    //Supprimer les pièces
    Graph::clear(m_jocker);
    List::Node<Graph::Node*>* it = m_pieces;
    while(it)
    {
        delete it->info;
        it = it->next;
    }
    List::clear(m_pieces);
}

Game::~Game()
{
    clear();
}
