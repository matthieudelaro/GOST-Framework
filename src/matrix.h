#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

template <typename T>
class Matrix : public Vector<T>
{
public:
    Matrix(unsigned int height = 0, unsigned int width = 0) : Vector<T>()
    {
        resize(height, width);
    }

    Matrix(unsigned int height, unsigned int width, const T &value) : Vector<T>()
    {
        resize(height, width, value);
    }

    inline T& operator()(unsigned int line, unsigned int column) {return (*this)[getIndex(line, column)];}
    inline T operator()(unsigned int line, unsigned int column) const {return (*this)[getIndex(line, column)];}
    inline unsigned int getHeight() const { return m_height; }
    inline unsigned int getWidth() const { return m_width; }
    inline unsigned int getIndex(unsigned int line, unsigned int column) const { return line*m_width + column; }

    void resize(unsigned int height, unsigned int width)
    {
        Vector<T>::resize(width*height);
        m_height = height;
        m_width = width;
    }

    inline void resize(unsigned int height, unsigned int width, T value)
    {
        Vector<T>::resize(width*height, value);
        m_height = height;
        m_width = width;
    }

    /*! \brief Réinitialise la Matrix.
     */
    virtual void clear()
    {
        Vector<T>::clear();
        m_height = 0;
        m_width = 0;
    }

private:
    unsigned int m_height, m_width;
};

#endif // MATRIX_H
