#include "graph.h"
#include <QDebug>

void Graph::clear(Graph::Node* &g)
{
    //Méthode simplifiée
    List::Node<Graph::Node*> *list = toList(g);
    List::clearDelete(list);
    g = NULL;

    //Méthode initiale (semble fonctionnelle) qui fait redondance avec Graph::toList()
    /*if(g)// <=> si ce Node != NULL
    {
        //on supprime les liens des voisins de g vers g
        for(unsigned int link = 0; link < Node::nbLinks; ++link)
        {
            for(unsigned int linkOfLink = 0; linkOfLink < Node::nbLinks; ++linkOfLink)
            {
                //si le voisin (*g)[link] != NULL
                //ET si ls voisin (*((*g)[link]))[linkOfLink] de (*g)[link] == g
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
        g = NULL;
    }*/


}

Graph::Node* Graph::copy(Graph::Node* original)
{
    Graph::Node *final = NULL;
    List::Node<Graph::Node*> *blacklist = NULL;
    copy(original, final, blacklist);
    return final;
}

void Graph::copy(Graph::Node* original, Graph::Node* &final, List::Node<Graph::Node*>* &blacklist)
{
    if(original && !List::contains(original, (const List::Node<Graph::Node*>*) blacklist))
    {
        final = new Node(original->info);
        List::push_front(final, blacklist);
        for(unsigned int link = 0; link < Node::nbLinks; ++link)
        {
            copy((*original)[link], (*final)[link], blacklist);
        }
    }
}

List::Node<Graph::Node*>* Graph::toList(Graph::Node* graph)
{
    List::Node<Graph::Node*> *list = NULL;
    Graph::toList(graph, list);
    return list;
}

List::Node<const Graph::Node*>* Graph::toConstList(const Graph::Node *graph)
{
    List::Node<const Graph::Node*> *list = NULL;
    Graph::toConstList(graph, list);
    return list;
}

void Graph::toList(Graph::Node* graph, List::Node<Graph::Node*>* &list)
{
    if(graph && !List::contains(graph, list))
    {
        List::push_front(graph, list);
        for(unsigned int link = 0; link < Node::nbLinks; ++link)
        {
            toList(graph->getConstLink(link), list);
        }
    }
}

void Graph::toConstList(const Graph::Node *graph, List::Node<const Graph::Node*> *&list)
{
    if(graph && !List::contains<const Graph::Node*>(graph, list))
    {
        List::push_front<const Graph::Node*>(graph, list);
        for(unsigned int link = 0; link < Graph::Node::nbLinks; ++link)
        {
            toConstList(graph->getConstLink(link), list);
        }
    }
}

void Graph::tests()
{
    qDebug() << "\ntestsGraph :";
    Graph::Node *v = new Graph::Node(0);
    v->getLink(0) = new Graph::Node(1);
    v->getLink(1) = new Graph::Node(2);
    v->getLink(2) = new Graph::Node(3);
    v->getLink(3) = new Graph::Node(4);

    v->getLink(0)->getLink(0) = new Graph::Node(11);
    v->getLink(0)->getLink(1) = new Graph::Node(12);
    v->getLink(0)->getLink(2) = new Graph::Node(13);
    v->getLink(0)->getLink(3) = new Graph::Node(14);

    v->getLink(1)->getLink(0) = new Graph::Node(21);
    v->getLink(1)->getLink(1) = new Graph::Node(22);
    v->getLink(1)->getLink(2) = new Graph::Node(23);
    v->getLink(1)->getLink(3) = new Graph::Node(24);

    v->getLink(2)->getLink(0) = new Graph::Node(31);
    v->getLink(2)->getLink(1) = new Graph::Node(32);
    v->getLink(2)->getLink(2) = new Graph::Node(33);
    v->getLink(2)->getLink(3) = new Graph::Node(34);

    v->getLink(3)->getLink(0) = new Graph::Node(41);
    v->getLink(3)->getLink(1) = new Graph::Node(42);
    v->getLink(3)->getLink(2) = new Graph::Node(43);
    v->getLink(3)->getLink(3) = new Graph::Node(44);

    qDebug() << "v : ";
    List::Node<Graph::Node*> *it, *listV = Graph::toList(v);
    it = listV;
    while(it)
    {
        qDebug() << it->info->info;
        it = it->next;
    }

    qDebug() << "w = copy(v) : ";
    Graph::Node *w = Graph::copy(v);
    List::Node<Graph::Node*> *listW = Graph::toList(w);
    it = listW;
    while(it)
    {
        qDebug() << it->info->info;
        it = it->next;
    }

    List::clear(listV);
    List::clear(listW);
    qDebug() << "clear(v) : ";
    Graph::clear(v);
    qDebug() << "clear(w) : ";
    Graph::clear(w);
}
