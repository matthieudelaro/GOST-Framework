#ifndef GRAPH_H
#define GRAPH_H

#include <cstddef> //pour avoir NULL
#include "list.h"

namespace Graph
{
    class Node
    {
    public :
        Node()
        {
            for(unsigned int i = 0; i < nbLinks; ++i)
                m_links[i] = NULL;
        }

        Node(unsigned int i) : info(i)
        {
            for(unsigned int i = 0; i < nbLinks; ++i)
                m_links[i] = NULL;
        }

        ~Node()
        {

        }

        static const unsigned int nbLinks = 4;
        inline Node*& operator[](unsigned int direction) {return m_links[direction];}
        inline Node* getConstLink(unsigned int direction) const {return m_links[direction];}
        inline Node*& getLink(unsigned int direction) {return m_links[direction];}

        unsigned int info;
    private :
        Node* m_links[nbLinks];
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

    inline unsigned int oppositeDirection(unsigned int direction) {return Node::nbLinks - direction;}

    void tests();
}

#endif // GRAPH_H
