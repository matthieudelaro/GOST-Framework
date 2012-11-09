#ifndef MATRIX_H
#define MATRIX_H

#include "tools.h"

template <typename T>
class Matrix
{
public:
    Matrix(unsigned int height = 0, unsigned int width = 0) : m_tab(NULL)
    {
        resize(height, width);
    }

    inline T& operator()(unsigned int line, unsigned int column) {return m_tab[getIndex(line, column)];}
    inline T& operator()(unsigned int index) { return m_tab[index];}
    inline T& operator[](unsigned int index) { return m_tab[index];}
    inline unsigned int getHeight() const { return m_height; }
    inline unsigned int getWidth() const { return m_width; }
    inline unsigned int getLength() const { return m_width*m_height; }
    inline unsigned int getIndex(unsigned int line, unsigned int column) const { return line*m_width + column; }

    void resize(unsigned int height, unsigned int width)
    {
        clear();
        m_height = height;
        m_width = width;
        if(m_height != 0 && m_width != 0)
        {
            m_tab = new T [m_height*m_width];
            if(m_tab == NULL) //si l'allocation a échoué (parce qu'il n'y a plus de RAM libre par exemple)
            {
                BadAllocation exception("Unable to allocate memory to resize Matrix.");
                throw exception;
            }
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
            unsigned int length = getLength();
            for(unsigned int i = 0; i < length; ++i)
                delete m_tab[i];
            delete [] m_tab;
            m_tab = NULL;
        }
    }

    ~Matrix()
    {
        clear();
    }

private:
    T *m_tab;
    unsigned int m_height, m_width;
};

#endif // MATRIX_H
