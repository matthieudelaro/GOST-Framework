#include "graph.h"

void Graph::clear(Graph::Node* g)
{
    if(g)// <=> si ce Node != NULL
    {
        //on supprime les liens des voisins de g vers g
        for(unsigned int link = 0; link < Node::nbLinks; ++link)
        {
            for(unsigned int linkOfLink = 0; linkOfLink < Node::nbLinks; ++linkOfLink)
            {
                //si le voisin (*g)[link] != NULL
                //ET si les voisin (*((*g)[link]))[linkOfLink] de (*g)[link] == g
                if( (*g)[link] && (*((*g)[link]))[linkOfLink] == g)
                    (*((*g)[link]))[linkOfLink] = NULL;//alors on le met à NULL
            }
        }
        //clear les voisins de g
        for(unsigned int link = 0; link < Node::nbLinks; ++link)
        {
            clear( (*g)[link] );
        }

        //puis on supprime g
        delete g;
    }
}
