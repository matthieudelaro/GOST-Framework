#ifndef GRAPH_H
#define GRAPH_H

#include <cstddef> //pour avoir NULL
#include "list.h"

/*! \brief Le namespace Graph englobe tout ce qui concerne les Graphs.
 *
 */
namespace Graph
{
    /*!
     * \brief La class Node représente un noeud du Graph.
     */
    class Node
    {
    public :
        /*!
         * \brief Ce constructeur initialise tous les Nodes voisins à NULL.
         */
        Node()
        {
            for(unsigned int i = 0; i < nbLinks; ++i)
                m_links[i] = NULL;
        }

        /*!
         * \brief Ce constructeur initialise tous les Nodes voisins à NULL, et l'info à i.
         */
        Node(unsigned int i) : info(i)
        {
            for(unsigned int i = 0; i < nbLinks; ++i)
                m_links[i] = NULL;
        }

        /*!
         * \brief Le nombre de liens vers des voisins.
         *
         *  Tous les Nodes ont nbLinks pointeurs vers des Nodes voisins. Si un pointeur vaut NULL
         *  alors il n'y a pas de voisin dans cette direction.
         *
         *  Pour utiliser
         *
         *  \see m_links
         */
        static const unsigned int nbLinks = 4;
        inline Node*& operator[](unsigned int direction) {return m_links[direction];}
        inline Node* getConstLink(unsigned int direction) const {return m_links[direction];}
        inline Node*& getLink(unsigned int direction) {return m_links[direction];}

        unsigned int info;///< Représente l'information contenue dans le Node.
    private :
        Node* m_links[nbLinks];///< Représente les liens vers les Nodes voisin du Node.
    };

    void clear(Node *&g);

    /*! \brief Cette fonction renvoie une copie du Graph.
     */
    Node* copy(Node *original);

    /*! \brief Cette fonction récursive est utilisée par Node* copy(Node *original). Elle n'est pas dédiée à l'utilisateur
     *  \see Node* copy(Node *original);
     */
    void copy(Graph::Node* original, Graph::Node* &final, List::Node<Graph::Node*>* &blacklist);

    /*! \brief Cette fonction renvoie une liste contenant les informations des noeuds du graphe.
     */
    List::Node<Graph::Node*>* toList(Graph::Node *graph);

    /*! \brief Cette fonction renvoie une liste const contenant les informations des noeuds du graphe.
     */
    List::Node<const Graph::Node*>* toConstList(const Graph::Node *graph);

    /*! \brief Cette fonction récursive est utilisée par toList(Graph::Node* graph). Elle n'est pas dédiée à l'utilisateur
     *  \see List::Node<unsigned int>* toList(Graph::Node* graph);
     */
    void toList(Graph::Node *graph, List::Node<Graph::Node*> *&list);

    /*! \brief Cette fonction récursive est utilisée par toConstList(Graph::Node* graph). Elle n'est pas dédiée à l'utilisateur
     *  \see List::Node<unsigned int>* toList(Graph::Node* graph);
     */
    void toConstList(const Graph::Node *graph, List::Node<const Graph::Node*> *&list);

    void tests();
}

#endif // GRAPH_H
