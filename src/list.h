#ifndef LIST_H
#define LIST_H

#include "collection.h"

namespace List
{
    template <typename T>
    struct Node
    {
        Node<T> *next;
        T info;
    };

    template <typename T>
    void clear(Node<T> *l)
    {
        if(l)
        {
            clear(l->next);
            delete l;
        }
    }

    template <typename T>
    void push_front(const T &info, Node<T> *l)
    {
        Node<T> *buffer = l;
        l = new Node<T>;
        l->info = info;
        l->next = buffer;
    }

    template<typename Key, typename Info>
    Node<Pair<Key, Info> >* find(const Key &key, const Node<Pair<Key, Info> >* n)
    {
        if(n == NULL)
            return NULL;
        else if(n->info.first == key)
            return n;
        else
            return find(key, n->next);
    }

    template<typename Key, typename Info1, typename Info2>
    Node<Triple<Key, Info1, Info2> >* find(const Key &key, Node<Triple<Key, Info1, Info2> >* n)
    {
        if(n == NULL)
            return NULL;
        else if(n->info.first == key)
            return n;
        else
            return find(key, n->next);
    }
}

#endif // LIST_H
