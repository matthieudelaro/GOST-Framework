#ifndef LIST_H
#define LIST_H

#include "collection.h"
#include <QDebug>

namespace List
{
    template <typename T>
    struct Node
    {
        Node<T> *next;
        T info;
    };

    template <typename T>
    void clear(Node<T>* &l)
    {
        Node<T> *it = l, *curr;
        while(it)
        {
            curr = it;
            it = it->next;
            delete curr;
        }
        l = NULL;
    }

    template <typename T>
    void clearDelete(Node<T>* &l)
    {
        Node<T> *it = l, *curr;
        while(it)
        {
            curr = it;
            it = it->next;

            delete curr->info;
            delete curr;
        }
        l = NULL;
    }

    template <typename T>
    bool contains(const T& researched, const Node<T>* l)
    {
        if(l)
        {
            if(l->info == researched)
                return true;
            else
                return contains(researched, l->next);
        }
        else
            return false;
    }

    template <typename T>
    void push_front(const T &info, Node<T>* &l)
    {
        Node<T> *buffer = l;
        l = new Node<T>;
        l->info = info;
        l->next = buffer;
    }

    template <typename T>
    void pop_front(Node<T>* &l)
    {
        if(l)
        {
            if(l->next)
            {
                Node<T> *buffer = l;
                l = l->next;
                delete buffer;
            }
            else
            {
                delete l;
                l = NULL;
            }
        }
    }

    template <typename T>
    void push_front(Node<T>* &l1, Node<T>* &l2)
    {
        Node<T> *it = l1;
        while(it->next != NULL)
            it = it->next;

        it->next = l2;
    }

    template <typename T>
    unsigned int size(const Node<T>* l)
    {
        if(l)
            return 1 + size(l->next);
        else
            return 0;
    }

    template<typename T>
    Node<T>* find(const T &researched, const Node<T>* l)
    {
        if(l == NULL)
            return NULL;
        else if(l->info == researched)
            return l;
        else
            return find(researched, l->next);
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

    template<typename T>
    void tests()
    {
        qDebug() << "\ntestsList : ";
        List::Node<T>* l = NULL;
        List::push_front(1, l);
        List::push_front(2, l);
        List::pop_front(l);
        List::push_front(3, l);

        List::Node<T>* l2 = NULL;
        List::push_front(4, l2);
        List::push_front(5, l2);
        List::push_front(6, l2);

        List::push_front(l,l2);

        qDebug() << "\nl :";
        List::Node<T>* it = l;
        while(it)
        {
            qDebug() << it->info;
            it = it->next;
        }
/*
        qDebug() << "size(l) = " << List::size(l);
        qDebug() << "clear(l);";
        List::clear(l);
        qDebug() << "size(l) = " << List::size(l);
        qDebug() << "l :";
        it = l;
        while(it)
        {
            qDebug() << it->info;
            it = it->next;
        }*/
    }
}

#endif // LIST_H
