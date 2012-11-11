#ifndef VECTOR_H
#define VECTOR_H

#include "tools.h"

/*! \brief Le classe Vector gère un tableau générique à taille dynamique.
 *
 *
 */
template <typename T>
class Vector
{
public:
    /*! \brief Ce constructeur initialise la taille du tableau à la valeur de length.
     *
     *  \param lenght Définit la taille du tableau. Sa valeur par défaut est 0.
     */
    Vector(unsigned int length = 0) : m_tab(NULL)
    {
        resize(length);
    }

    /*! \brief Ce constructeur initialise la taille du tableau à la valeur de length, en remplissant toutes les cases avec value.
     *
     *  \param lenght Définit la taille du tableau. Sa valeur par défaut est 0.
     *  \param value La valeur que prendra chaque case du tableau.
     */
    Vector(unsigned int length, const T &value) : m_tab(NULL)
    {
        resize(length, value);
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
     *  \return La valeur contenue à l'index.
     */
    inline T operator[](unsigned int index) const { return m_tab[index];}

    /*! \return La longueur du tableau.
     */
    inline unsigned int getLength() const { return m_length; }

    /*! \brief Redimensionne le tableau.
     *
     *  \param length Définit la taille du tableau.
     */
    void resize(unsigned int length)
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
    void clear()
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
            for(unsigned int i = 0; i < m_length; ++i)
                delete m_tab[i];
            delete [] m_tab;
            m_tab = NULL;
        }
    }

    /*! \brief Convertit le Vector en QString.
     *
     *  Exemple : Pour un Vector<int> contenant 1, 5 et 6, la fonction renvoie (1, 5, 6).
     *
     *  \see Vector<T>::clear()
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
    ~Vector()
    {
        clear();
    }

private:
    T *m_tab; ///< tableau dynamique stockant les valeurs
    unsigned int m_length; ///< longueur du tableau
};

#endif // VECTOR_H
