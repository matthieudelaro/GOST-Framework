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
    inline unsigned int getHeight() const { return m_height; }
    inline unsigned int getWidth() const { return m_width; }
    inline unsigned int getIndex(unsigned int line, unsigned int column) const { return line*m_width + column; }

    void resize(unsigned int height, unsigned int width)
    {
        m_height = height;
        m_width = width;
        Vector<T>::resize(m_width*m_height);
    }

    inline void resize(unsigned int height, unsigned int width, T value)
    {
        m_height = height;
        m_width = width;
        Vector<T>::resize(m_width*m_height, value);
    }

private:
    unsigned int m_height, m_width;
};

#endif // MATRIX_H
