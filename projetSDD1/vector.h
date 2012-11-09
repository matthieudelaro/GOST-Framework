#ifndef VECTOR_H
#define VECTOR_H

#include "tools.h"

template <typename T>
class Vector
{
public:
    Vector(unsigned int length = 0) : m_tab(NULL)
    {
        resize(length);
    }

    Vector(unsigned int length, const T &value) : m_tab(NULL)
    {
        resize(length, value);
    }

    inline T& operator()(unsigned int index) { return m_tab[index];}
    inline T& operator[](unsigned int index) { return m_tab[index];}
    inline T operator[](unsigned int index) const { return m_tab[index];}
    inline unsigned int getLength() const { return m_length; }

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

    void resize(unsigned int length, const T &value)
    {
        resize(length);
        for(unsigned int i = 0; i < m_length; ++i)
        {
            m_tab[i] = value;
        }
    }

    void clear()
    {
        if(m_tab)
        {
            delete [] m_tab;
            m_tab = NULL;
        }
    }

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

    ~Vector()
    {
        clear();
    }

private:
    T *m_tab;
    unsigned int m_length;
};

#endif // VECTOR_H
