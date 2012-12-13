#ifndef VECTOR_H
#define VECTOR_H

#include "tools.h"
#include <QDebug>

namespace Gost
{
/*! \brief Le classe Vector gère un tableau générique à taille dynamique.
 */
template <typename T>
class Vector
{
public:
    /*!
     * \brief Le constructeur de copie permet de copier un Vector.
     * \param original Le Vector copié.
     * \return Une copie d'original
     */
    Vector& operator=(const Vector& original)
    {
        m_tab = NULL;
        resize(original.getLength());
        for(unsigned int i = 0; i < m_length; ++i)
            m_tab[i] = original[i];
        return *this;
    }

    /*! \brief Ce constructeur initialise la taille du tableau à la valeur de length.
     *
     *  \param length Définit la taille du tableau. Sa valeur par défaut est 0.
     */
    Vector(unsigned int length = 0) : m_tab(NULL)
    {
        resize(length);
    }

    /*! \brief Ce constructeur initialise la taille du tableau à la valeur de length, en remplissant toutes les cases avec value.
     *
     *  \param length Définit la taille du tableau. Sa valeur par défaut est 0.
     *  \param value La valeur que prendra chaque case du tableau.
     */
    Vector(unsigned int length, const T &value) : m_tab(NULL)
    {
        resize(length, value);
    }

    /*! \brief Constructeur de copie. Recopie l'intégralité de l'ogininal dans un nouveau tableau.
     *
     *  \param original Le Vector copié.
     */
    Vector(const Vector<T> &original) : m_tab(NULL)
    {
        resize(original.getLength());
        for(unsigned int i = 0; i < m_length; ++i)
            m_tab[i] = original[i];
    }

    /*! \brief Permet d'accéder aux éléments du tableau.
     *
     *  \param index L'index dont on veut connaître la valeur.
     *  \return Une référence vers la valeur contenue à l'index.
     */
    inline T& operator[](unsigned int index) { return m_tab[index];}

    /*! \brief Permet d'accéder aux éléments du tableau.
     *
     *  \param index L'index dont on veut connaître la valeur.
     *  \return Une référence vers la valeur contenue à l'index.
     *  \see inline const T& operator[](unsigned int index) const
     */
    inline T& get(unsigned int index) { return m_tab[index];}

    /*! \brief Permet d'accéder aux éléments du tableau.
     *
     *  \param index L'index dont on veut connaître la valeur.
     *  \return La valeur contenue à l'index.
     *  \see inline T& get(unsigned int index)
     */
    inline const T& operator[](unsigned int index) const { return m_tab[index];}

    /*! \brief Permet de consulter les éléments du tableau.
     *
     *  \param index L'index dont on veut connaître la valeur.
     *  \return Une référence constante vers la valeur contenue à l'index.
     *  \see inline const T& operator[](unsigned int index) const
     */
    inline const T& getConst(unsigned int index) const { return m_tab[index];}

    /*! \return La longueur du tableau.
     */
    inline unsigned int getLength() const { return m_length; }

    /*! \brief Redimensionne le tableau.
     *
     *  \param length Définit la taille du tableau.
     */
    virtual void resize(unsigned int length)
    {
        clear();
        m_length = length;
        if(m_length != 0)
        {
            m_tab = new T [m_length];
            if(m_tab == NULL) //si l'allocation a échoué (parce qu'il n'y a plus de RAM libre par exemple)
            {
                BadAllocation exception("Unable to allocate memory to resize Vector.");
                throw exception;
            }
        }
    }


    /*! \brief Redimensionne le tableau.
     *
     *  \param length Définit la taille du tableau.
     *  \param value Valeur des nouvelles cases.
     */
    void resize(unsigned int length, const T &value)
    {
        resize(length);
        for(unsigned int i = 0; i < m_length; ++i)
        {
            m_tab[i] = value;
        }
    }

    /*! \brief Réinitialise le Vector. Vide le tableau dynamique, et le redimensionne à 0 cases.
     */
    virtual void clear()
    {
        if(m_tab)
        {
            delete [] m_tab;
            m_tab = NULL;
        }
        m_length = 0;
    }

    /*! \brief Réinitialise le Vector.
     *
     *  Vide le tableau dynamique et delete tous ses éléments
     *  (utile dans le cas d'un Vector de pointeurs), et le redimensionne à 0 cases.
     *
     *  \see Vector<T>::clear()
     */
    void clearDelete()
    {
        if(m_tab)
        {
            //on supprime chaque case
            for(unsigned int i = 0; i < m_length; ++i)
                if(m_tab[i])//si elle n'est pas à NULL
                    delete m_tab[i];
            delete [] m_tab;//puis on supprime le tableau
            m_tab = NULL;
        }
    }

    /*! \brief Convertit le Vector en QString.
     *
     *  Exemple : Pour un Vector<int> contenant 1, 2 et 3, la fonction renvoie (1, 2, 3).
     *
     *  \return Un QString contenant une représentation textuelle du contenu du Vector.
     */
    QString toString() const
    {
        QString out;
        if(getLength() == 1)
            out = QString("(%1)").arg(m_tab[1]);
        else if(getLength() >= 2)
        {
            out += "(";
            for(unsigned int i = 0; i < getLength() - 1; ++i)
            {
                out += QString("%1, ").arg(m_tab[i]);
            }
            out += QString("%1)").arg(m_tab[getLength() - 1]);
        }

        return out;
    }

    /*! \brief Appelle clear().
     */
    virtual ~Vector()
    {
        clear();
    }

    /*!
     * \brief Implémente une série de tests pour tester la classe Vector de manière autonome.
     */
    static void tests()
    {
        qDebug() << "\ntestsVector :";
        Vector<int> v;
        v.resize(3);
        v[0] = 5;
        v[1] = 6;
        v[2] = 7;
        qDebug() << "v : " << v.toString();

        Vector<int> w(v);
        qDebug() << "w(v) : " << w.toString();

        Vector<int> x;
        x = w;
        qDebug() << "x = w : " << x.toString();

//        Vector<int> a(5, 5);
//        qDebug() << (a == v);
//        a.resize(3, 5);
//        qDebug() << (a == v);
//        a[0] = 5;
//        a[1] = 6;
//        a[2] = 7;
//        qDebug() << (a == v);
    }

private:
    T *m_tab; ///< tableau dynamique stockant les valeurs
    unsigned int m_length; ///< longueur du tableau
};

/*!
 *  \brief Permet de comparer unes à unes les cases de deux Vector.
 *  \return true si les cases sont toutes égales une à une, false sinon.
 */
template <typename T>
bool operator==(const Vector<T>& v, const Vector<T>& w)
{
    if(v.getLength() != w.getLength())
        return false;//on renvoie false si ils n'ont pas la même longueur
    for(unsigned int i = 0; i < v.getLength(); ++i)
        if(v.getConst(i) != w.getConst(i))
            return false;//on renvoie false si toutes les cases ne sont pas identiques 2 à 2
    return true;//si tout va bien, on renvoie true
}

}

#endif // VECTOR_H
