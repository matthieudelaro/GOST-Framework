#include "game.h"

#include "tools.h"

Game::Game(QDomDocument &xml)
{
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
            QDomNodeList listLines = xml.elementsByTagName("line");
            if(!tempListcolumns.isEmpty() && !listLines.isEmpty())
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

                for(unsigned int line = 0; line < nbLines; ++line)
                {
                    QDomNodeList columns = listLines.item(line).childNodes();
                    unsigned int nbColumns = ((unsigned int)columns.count());
                    if(nbColumns > nbMaxColumns)
                        nbMaxColumns = nbColumns;
                    for(unsigned int column = 0; column < nbColumns; ++column)
                    {
                        QDomElement element = columns.item(column).toElement();
                        if(element.attribute("type") != "void")
                        {
                            m_nbNodes++;
                            if(element.attribute("type") == "piece")
                            {
                                if(!List::find(element.attribute("number").toInt(), pieces))
                                {
                                    Triple<int, Matrix<Graph::Node*>, Graph::Node*> piece;
                                    piece.first = element.attribute("number").toInt();
                                    piece.third = NULL;
                                    List::push_front(piece, pieces);
                                }
                            }
                        }
                    }
                }

                m_index = new Graph::Node* [m_nbNodes];
                List::Node<Triple<int, Matrix<Graph::Node*>, Graph::Node*> > *it = pieces;
                while(it)
                {
                    it->info.second.resize(nbLines, nbMaxColumns);
                    it = it->next;
                }
                //Matrix<Graph::Node*> tabBoard(nbLines, nbMaxColumns);
                m_boardMatrix.resize(nbLines, nbMaxColumns);

                //Etape 2 :
                //Parcours du XML
                //Remplissage des tab 2D
                unsigned int index = 0;
                for(unsigned int line = 0; line < nbLines; ++line)
                {
                    QDomNodeList columns = listLines.item(line).childNodes();
                    unsigned int nbColumns = ((unsigned int)columns.count());
                    for(unsigned int column = 0; column < nbColumns; ++column)
                    {
                        it = pieces;

                        QDomElement element = columns.item(column).toElement();
                        if(element.attribute("type") == "void")
                        {
                            m_boardMatrix(line, column) = NULL;
                            while(it)
                            {
                                it->info.second(line, column) = NULL;
                                it = it->next;
                            }
                        }
                        else
                        {
                            m_boardMatrix(line, column) = new Graph::Node(m_boardMatrix.getIndex(line, column));
                            if(m_board == NULL)
                                m_board = m_boardMatrix(line, column);
                            m_index[index] = m_boardMatrix(line, column);
                            index++;
                            if(element.attribute("type") == "piece")
                            {
                                while(it)
                                {
                                    if(element.attribute("number").toInt() == it->info.first)
                                    {
                                        //on créé un noeud dont la valeur correspond au numéro de la pièce
                                        it->info.second(line, column) = new Graph::Node(it->info.first);

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
                            //Cette partie est spécifique à un système à base carrée !
                            //*((*(m_boardMatrix(line, column)))[0]) = m_boardMatrix(line-1, column);
                            if(line > 0)
                                (*(m_boardMatrix(line, column)))[0] = m_boardMatrix(line-1, column);
                            if(line + 1 < m_boardMatrix.getHeight())
                                (*(m_boardMatrix(line, column)))[1] = m_boardMatrix(line+1, column);
                            if(column > 0)
                                (*(m_boardMatrix(line, column)))[2] = m_boardMatrix(line, column-1);
                            if(column + 1 < m_boardMatrix.getWidth())
                                (*(m_boardMatrix(line, column)))[3] = m_boardMatrix(line, column+1);

                            //Pour avoir quelque chose de plus générique :
                            //for(unsigned int link = 0; link < Graph::nbLinks; ++link)
                            //    m_boardMatrix(line, column)[link] = ?
                        }

                        //on fait la même chose pour chaque pièce :
                        it = pieces;
                        while(it)
                        {
                            if(it->info.second(line, column))
                            {
                                if(line > 0)
                                    (*(it->info.second(line, column)))[0] = it->info.second(line-1, column);
                                if(line + 1 < it->info.second.getHeight())
                                    (*(it->info.second(line, column)))[1] = it->info.second(line+1, column);
                                if(column > 0)
                                    (*(it->info.second(line, column)))[2] = it->info.second(line, column-1);
                                if(column + 1 < it->info.second.getWidth())
                                    (*(it->info.second(line, column)))[3] = it->info.second(line, column+2);
                            }
                            it = it->next;
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
                clear(pieces);
            }
            else
            {
                XMLFormatException exception("'line' and 'columns' nodes not found.");
                throw exception;//exception.raise();
            }
        }
        else
        {
            XMLFormatException exception("InitialShape type not supported.");
            throw exception;//exception.raise();
        }
    }
    else
    {
        XMLFormatException exception("'Board', initialShape', and/or 'finalShape' nodes not found.");
        throw exception;//exception.raise();
        //int exception = 10;
        //throw exception;
    }
}

const Graph::Node *& Game::getNodePiece(unsigned int index, const Matrix<Graph::Node *> &etat) const
{
    qDebug() << "Game::getNodePiece à implémenter !";
}

Game::~Game()
{
    delete [] m_index;
}

unsigned int Game::getNumberPiece(unsigned int index, const Matrix<Graph::Node *> &etat) const
{
    qDebug() << "Game::getNumberPiece à implémenter !";
    return 0;
}


/*void Lecture_DOM::lire()
{
    int i=0;
    QString affichage;
    QDomNodeList tab;
    QDomElement mesure;
    QDomNode n;
    QMessageBox a(0);
    QDomElement racine = doc.documentElement(); // renvoie la balise racine
    QDomNode noeud = racine.firstChild(); // renvoie la première balise « mesure »
    while(!noeud.isNull())
    {
        // convertit le nœud en élément pour utiliser les
        // méthodes tagName() et attribute()
        mesure = noeud.toElement();
        // vérifie la présence de la balise « mesure »
        if (mesure.tagName() == "mesure")
        {
            affichage = mesure.attribute("numero"); // récupère l'attribut
            tab = mesure.childNodes(); // crée un tableau des enfants de « mesure »
            for(i=0;i<tab.length();i++)
            {
                // pour chaque enfant, on extrait la donnée et on concatène
                n = tab.item(i);
                affichage = affichage + " " + n.firstChild().toText().data();
            }
            a.setText(affichage); // affichage dans un QMessageBox
            a.exec();
        }
        noeud = noeud.nextSibling(); // passe à la "mesure" suivante
    }
}*/
