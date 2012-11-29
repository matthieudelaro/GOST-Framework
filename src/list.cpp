/*#include "list.h"
#include <QDebug>

template <typename T>
void List::clear(List::Node<T>* &l)
{
    List::Node<T> *it = l, *curr;
    while(it)
    {
        curr = it;
        it = it->next;
        delete curr;
    }
    l = NULL;
}

template <typename T>
void List::clearDelete(List::Node<T>* &l)
{
    List::Node<T> *it = l, *curr;
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
bool List::contains(const T& researched, const List::Node<T>* l)
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
void List::push_front(const T &info, List::Node<T>* &l)
{
    List::Node<T> *buffer = l;
    l = new List::Node<T>;
    l->info = info;
    l->next = buffer;
}

template <typename T>
unsigned int List::size(const List::Node<T>* l)
{
    if(l)
        return 1 + size(l->next);
    else
        return 0;
}

template<typename Key, typename Info>
List::Node<Pair<Key, Info> >* List::find(const Key &key, const List::Node<Pair<Key, Info> >* n)
{
    if(n == NULL)
        return NULL;
    else if(n->info.first == key)
        return n;
    else
        return find(key, n->next);
}

template<typename Key, typename Info1, typename Info2>
List::Node<Triple<Key, Info1, Info2> >* List::find(const Key &key, List::Node<Triple<Key, Info1, Info2> >* n)
{
    if(n == NULL)
        return NULL;
    else if(n->info.first == key)
        return n;
    else
        return find(key, n->next);
}

void List::tests()
{
    qDebug() << "\ntestsList : ";
    List::Node<int>* l = NULL;
    List::push_front(1, l);
    List::push_front(2, l);
    List::push_front(3, l);

    qDebug() << "\nl :";
    List::Node<int>* it = l;
    while(it)
    {
        qDebug() << it->info;
        it = it->next;
    }
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
    }
}
*/
