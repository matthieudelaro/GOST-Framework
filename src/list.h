#ifndef LIST_H
#define LIST_H

#include "collection.h"
#include <QDebug>

namespace Gost
{
/*!
 *  \brief Le namespace List englobe tout ce qui concerne les listes.
 *  Tout est implémententé sur la base de template, ce qui assure une généricité maximale du code.
 */
namespace List
{
    /*!
     *  \brief La structure Node représente un maillon de la liste.
     */
    template <typename T>
    struct Node
    {
        Node<T> *next;///< Représente l'élément suivant de la liste.
        T info;///< Représente l'information contenue dans le maillon.
    };

    /*!
     *  \brief Vide la liste.
     *  \param l La liste que l'on veut vider.
     */
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

    /*!
     *  \brief Vide la liste, et "delete" les informations de chaque noeud.
     *  \param l La liste que l'on veut vider.
     */
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

    /*!
     *  \brief Permet de savoir si la liste contient un élément donné.
     *  \param researched L'élément que l'on recherche.
     *  \param l La liste dans laquelle on recherche.
     *  \return true si la liste contient l'élément, false sinon.
     */
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

    /*!
     * \brief Permet d'ajouter un élément au début de la liste.
     * \param info L'élément à ajouter à la liste.
     * \param l La liste à laquelle on ajoute l'élément.
     * \see void push_front(T &info, Node<T>* &l)
     */
    template <typename T>
    void push_front(const T &info, Node<T>* &l)
    {
        Node<T> *buffer = l;
        l = new Node<T>;
        l->info = info;
        l->next = buffer;
    }

    /*!
     * \brief Permet d'ajouter un élément non constant au début de la liste.
     * \param info L'élément à ajouter à la liste.
     * \param l La liste à laquelle on ajoute l'élément.
     * \see void push_front(const T &info, Node<T>* &l)
     */
    //push non const
    template <typename T>
    void push_front(T &info, Node<T>* &l)
    {
        Node<T> *buffer = l;
        l = new Node<T>;
        l->info = info;
        l->next = buffer;
    }

    /*!
     * \brief Permet de supprimer un élément au début de la liste.
     * \param l La liste dont on supprime le premier élément.
     */
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

    /*!
     * \brief Permet de supprimer un élément à la fin de la liste, et de le récupérer.
     * \param l La liste dont on supprime le premier élément.
     * \return L'élément supprimé.
     */
    template <typename T>
    Node<T>* pop_backAndReturn(Node<T>* &l)
    {
        if(!l)
            return l;
        Node<T> *it = l;
        if(it->next == NULL)//cas liste à un élément
        {
            return pop_frontAndReturn(it);
        }
        else
        {
            while(it->next->next != NULL)
            {
                it = it->next;
            }
            //on a la fin de la liste en l->next;
            Node<T> *toReturn = it->next;
            it->next = NULL;
            return toReturn;
        }
    }

    /*!
     * \brief Permet de supprimer un élément au début de la liste, et de le récupérer.
     * \param l La liste dont on supprime le premier élément.
     * \return L'élément supprimé.
     */
    template <typename T>
    Node<T>* pop_frontAndReturn(Node<T>* &l)
    {
        if(!l)
            return l;
        if(l->next)
        {
            Node<T> *buffer = l;
            l = l->next;
            return buffer;
        }
        return l;
    }

    /*!
     * \brief Permet de supprimer un élément au début de la liste, et de le récupérer.
     * \param l La liste dont on supprime le premier élément.
     * \return L'information de l'élément supprimé.
     */
    template <typename T>
    T pop_frontAndReturnValue(Node<T>* &l)
    {
        if(l == NULL)
            return NULL;
        if(l)
        {
            if(l->next)
            {
                Node<T> *buffer = l;
                l = l->next;
                T toReturn = buffer->info;
                delete buffer;
                return toReturn;
            }
            T toReturn = l->info;
            delete l;
            return toReturn;
        }
    }

    /*!
     *  \brief Permet d'ajouter une liste à la fin d'une autre.
     *  \param l1 La liste à laquelle on ajoute l2.
     *  \param l2 La liste qu'on ajoute à la fin de l1.
     */
    template <typename T>
    void push_front(Node<T>* &l1, Node<T>* &l2)
    {
        if(l1 == NULL)
        {
            l1 = l2;
        }
        else
        {
            if(l2 == NULL)
                return;
            else
            {
                Node<T>* lTmp = l2;
                while(lTmp->next != NULL) lTmp = lTmp->next;

                lTmp->next = l1;
                l1 = l2;
            }
        }
    }

    /*!
     *  \return La longueur de la liste.
     */
    template <typename T>
    unsigned int size(const Node<T>* l)
    {
        if(l)
            return 1 + size(l->next);
        else
            return 0;
    }

    /*!
     * \brief Permet de supprimer un maillon d'une liste.
     * \param toRemove L'élement à supprimer.
     * \param l la liste dans laquelle on recherche.
     */
    template<typename T>
    void remove(Node<T>* toRemove, Node<T>* l)
    {
        if(l == NULL || toRemove == NULL)
            return;
        if(toRemove == l) // cas de la tête de liste
        {
            l = l->next;
            toRemove->next = NULL;
            delete toRemove;
            toRemove = NULL;

        }
        else if(toRemove == l->next)
        {
            l->next = toRemove->next;
            toRemove->next = NULL;
            delete toRemove;
            toRemove = NULL;
        }
        else
        {
            remove(toRemove,l->next);
        }
    }

    /*!
     *  \brief Recherche un élément dans une liste.
     *  \param researched L'élément que l'on recherche.
     *  \param l la liste dans laquelle on recherche.
     *  \return NULL si l'élément n'est pas trouvé, un pointeur sur l'élément sinon.
     */
    template<typename T>
    Node<T>* find(const T &researched, Node<T>* l)
    {
        if(l == NULL)
            return NULL;
        else if(l->info == researched)
            return l;
        else
            return find(researched, l->next);
    }

    /*!
     *  \brief Recherche un élément dans une liste de Pairs, en fonction du premier membre du Pair.
     *
     *  Cette fonction permet de mimer le fonctionnement des std::map.
     *
     *  \param key L'élément que l'on recherche.
     *  \param n la liste dans laquelle on recherche.
     *  \return NULL si l'élément n'est pas trouvé, un pointeur sur l'élément sinon.
     */
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

    /*!
     *  \brief Recherche un élément dans une liste de Triple, en fonction du premier membre du Triple.
     *
     *  Cette fonction permet de mimer le fonctionnement des std::map.
     *
     *  \param key L'élément que l'on recherche.
     *  \param n la liste dans laquelle on recherche.
     *  \return NULL si l'élément n'est pas trouvé, un pointeur sur l'élément sinon.
     */
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

    /*!
     *  \brief Recherche un élément dans une liste de Quadruple, en fonction du premier membre du Quadruple.
     *
     *  Cette fonction permet de mimer le fonctionnement des std::map.
     *
     *  \param key L'élément que l'on recherche.
     *  \param n la liste dans laquelle on recherche.
     *  \return NULL si l'élément n'est pas trouvé, un pointeur sur l'élément sinon.
     */
    template<typename Key, typename Info1, typename Info2, typename Info3>
    Node<Quadruple<Key, Info1, Info2, Info3> >* find(const Key &key, Node<Quadruple<Key, Info1, Info2, Info3> >* n)
    {
        if(n == NULL)
            return NULL;
        else if(n->info.first == key)
            return n;
        else
            return find(key, n->next);
    }

    /*!
     *  \brief Recherche un élément dans une liste d'AStarNode, en fonction du premier membre de l'AStarNode.
     *
     *  Cette fonction permet de mimer le fonctionnement des std::map.
     *
     *  \param key L'élément que l'on recherche.
     *  \param n la liste dans laquelle on recherche.
     *  \return NULL si l'élément n'est pas trouvé, un pointeur sur l'élément sinon.
     */
    template<typename Key, typename Info1, typename Info2>
    Node<AStarNode<Key, Info1, Info2> >* find(const Key &key, Node<AStarNode<Key, Info1, Info2> >* n)
    {
        if(n == NULL)
            return NULL;
        else if(n->info.first == key)
            return n;
        else
            return find(key, n->next);
    }

    /*!
     *  \brief Retire l'élément toRemove de la liste l. Cet élément ne sera pas modifié (ni supprimé !).
     *  \param toRemove L'élément à retirer.
     *  \param l La liste dans laquelle on recherche toRemove.
     *  \return true si l'élément a été trouvé et retiré, false sinon.
     */
    template<typename T>
    bool removeElementFromList(Node<T>* toRemove, Node<T>* &l)
    {
        if(!l || !toRemove)
            return false;

        if(l == toRemove)
        {
            l = l->next;
            return true;
        }
        else
        {
            removeElementFromList(toRemove, l->next);
        }
    }


    /*!
     * \brief Implémente une série de tests pour tester le namespace de manière autonome.
     */
    template<typename T>
    void tests()
    {
        qDebug() << "\ntestsList : ";
        List::Node<T>* l = NULL;

        List::Node<T> *nodeToRemove = new  List::Node<T>;
        nodeToRemove->info = 10;
        nodeToRemove->next = NULL;

        List::Node<T> *l2 = new List::Node<T>;
        l2->info = 100;
        l2->next = new List::Node<T>;
        l2->next->info = 200;
        l2->next->next = new List::Node<T>;
        l2->next->next->info = 300;
        l2->next->next->next = NULL;

        List::push_front(1, l);
        List::push_front(2, l);
        List::pop_front(l);
        List::push_front(3, l);
        List::push_front(32, l);
        List::push_front(l, l2);
        //List::push_front(l,nodeToRemove);
        List::push_front(1, l);
        List::push_front(2, l);
        List::push_front(1, l);
        List::push_front(2, l);
        List::removeElementFromList(l2, l);





        //List::remove(nodeToRemove,l);

        qDebug() << "\nl :";
        List::Node<T>* it = l;
        while(it)
        {
            qDebug() << it->info;
            it = it->next;
        }

        it = l2;
        while(it)
        {
            qDebug() << it->info;
            it = it->next;
        }
/*
        List::Node<T>* test = List::pop_backAndReturn(l);
        qDebug() << "pop back :" << test->info;
        List::Node<T>* test2 = List::pop_frontAndReturn(l);
        qDebug() << "pop front :" << test2->info;


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
}
#endif // LIST_H
