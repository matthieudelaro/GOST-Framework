#ifndef GRAPH_H
#define GRAPH_H

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

        static const unsigned int nbLinks = 4;
        //inline Node** operator[](unsigned int direction) {return &(m_links[direction]);}
        inline Node*& operator[](unsigned int direction) {return m_links[direction];}
        //inline Node* getNext(unsigned int direction) const {return m_links[direction];}
        //inline void setNext()

        unsigned int info;
    private :
        Node* m_links[nbLinks];
    };

    /*class NodeIndex : public Node
    {
    public :
        NodeIndex(unsigned int index) : Node(), m_index(index) {}
        unsigned int m_index;
    };*/

    inline unsigned int oppositeDirection(unsigned int direction) {return Node::nbLinks - direction;}

}

#endif // GRAPH_H
