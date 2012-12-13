#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

/*! \brief Le classe Matrix gère un tableau 2D à taille variable.
 *
 *  Matrix se base sur un Vector pour stocker ses données. Toutes les données sont donc les unes à la suites des autres
 *  en mémoire, se qui accélère la vitesse d'accès des données lorsque l'on itère toutes les cases.
 */
template <typename T>
class Matrix
{
public:
    //le constructeur de copie et l'opérateur = sont bien implémentés dans Vector,
    //donc il n'y a plus besoin de les réimplémenter dans Matrix

    /*! \brief Ce constructeur initialise une Matrix de hauteur height et de largeur width.
     *
     *  \param height Définit la hauteur de la Matrix.
     *  \param width Définit la largeur de la Matrix.
     */
    Matrix(unsigned int height = 0, unsigned int width = 0)
    {
        resize(height, width);
    }

    /*! \brief Ce constructeur initialise une Matrix de hauteur height et de largeur width, contenant value dans toutes les cases.
     *
     *  \param height Définit la hauteur de la Matrix.
     *  \param width Définit la largeur de la Matrix.
     *  \param value Valeur des nouvelles cases.
     */
    Matrix(unsigned int height, unsigned int width, const T &value)
    {
        resize(height, width, value);
    }

    /*! \return Une référence vers la case (line, column)
     *  \see get(unsigned int line, unsigned int column)
     */
    inline T& operator()(unsigned int line, unsigned int column) {return m_vector.get(convertToIndex(line, column));}

    /*! \return Une référence vers la case (line, column).
     *  \see operator()(unsigned int line, unsigned int column)
     */
    inline T& get(unsigned int line, unsigned int column) {return m_vector.get(convertToIndex(line, column));}

    /*! \return Une référence vers la case index.
     */
    inline T& get(unsigned int index) {return m_vector[index];}

    /*! \return Une copie de la case (line, column).
     *  \see getConst(unsigned int line, unsigned int column) const
     */
    inline T operator()(unsigned int line, unsigned int column) const {return m_vector[convertToIndex(line, column)];}

    /*! \return Une référence constante vers la case (line, column).
     */
    inline const T& getConst(unsigned int line, unsigned int column) const {return m_vector.getConst(convertToIndex(line, column));}

    /*! \return Une référence constante vers la case index.
     */
    inline const T& getConst(unsigned int index) const {return m_vector.getConst(index);}

    /*! \return La hauteur de la Matrix.
     */
    inline unsigned int getHeight() const { return m_height; }

    /*! \return La largeur de la Matrix.
     */
    inline unsigned int getWidth() const { return m_width; }

    /*! \return Le nombre de cases dans la Matrix.
     */
    inline unsigned int getLength() const { return m_vector.getLength(); }

    /*! \return Le Vector utilisé par la Matrix. Utilise pour itérer les éléments un à un.
     */
    inline const Vector<T>& getVector() const {return m_vector;}

    /*! \brief Convertit des coordonnées 2D en un index pour un tableau 1D.
     */
    inline unsigned int convertToIndex(unsigned int line, unsigned int column) const { return line*m_width + column; }

    /*! \brief Vérifie que la case (line, column) existe.
     *  \see inRange(unsigned int index)
     */
    inline bool inRange(unsigned int line, unsigned int column) const { return line < m_height && column < m_width; }

    /*! \brief Vérifie que la case (index) existe.
     *  \see inRange(unsigned int line, unsigned int column)
     */
    inline bool inRange(unsigned int index) const { return index < m_vector.getLength(); }

    /*! \brief Redimensionne la Matrix.
     *
     *  \param height Définit la hauteur de la Matrix.
     *  \param width Définit la largeur de la Matrix.
     */
    void resize(unsigned int height, unsigned int width)
    {
        m_vector.resize(width*height);
        m_height = height;
        m_width = width;
    }

    /*! \brief Redimensionne la Matrix.
     *
     *  \param height Définit la hauteur de la Matrix.
     *  \param width Définit la largeur de la Matrix.
     *  \param value Valeur des nouvelles cases.
     */
    inline void resize(unsigned int height, unsigned int width, T value)
    {
        m_vector.resize(width*height, value);
        m_height = height;
        m_width = width;
    }

    /*! \brief Réinitialise la Matrix.
     */
    virtual void clear()
    {
        m_vector.clear();
        m_height = 0;
        m_width = 0;
    }

    /*! \brief Réinitialise la Matrix.
     *
     *  Vide m_vector de tous ses éléments
     *  (utile dans le cas d'une Matrix de pointeurs), et le redimensionne à (0, 0) cases.
     *
     *  \see Matrix<T>::clear()
     */
    void clearDelete()
    {
        m_vector.clearDelete();
        m_height = 0;
        m_width = 0;
    }

    /*! \brief Convertit la Matrix en QString.
     *
     *  Exemple : Pour un Matrix<int> de dimension (2, 2) contenant 1, 2, 3 et 4, la fonction renvoie (1, 2, 3, 4).
     *
     *  \return Un QString contenant une représentation textuelle du contenu de la Matrix.
     */
    inline QString toString() const
    {
        return m_vector.toString();
    }

    /*!
     * \brief Implémente une série de tests pour tester la class Matrix de manière autonome.
     */
    static void tests()
    {
        qDebug() << "\ntestsMatrix :";
        Matrix<int> v;
        v.resize(2, 2);
        v(0, 0) = 5;
        v(0, 1) = 6;
        v(1, 0) = 7;
        v(1, 1) = 8;
        qDebug() << "v : " << v.toString();
        qDebug() << "v.inRange(-1) : " << v.inRange(-1);
        qDebug() << "v.inRange(0) : " << v.inRange(0);
        qDebug() << "v.inRange(1) : " << v.inRange(1);
        qDebug() << "v.inRange(2) : " << v.inRange(2);
        qDebug() << "v.inRange(3) : " << v.inRange(3);
        qDebug() << "v.inRange(4) : " << v.inRange(4);
        qDebug() << "v.inRange(5) : " << v.inRange(5);
        qDebug() << "v.inRange(1, 1) : " << v.inRange(1, 1);
        qDebug() << "v.inRange(1, 2) : " << v.inRange(1, 2);
        qDebug() << "v.inRange(3, 0) : " << v.inRange(3, 0);
        qDebug() << "v.inRange(0, 3) : " << v.inRange(0, 3);

        Matrix<int> w(v);
        qDebug() << "w(v) : " << w.toString();

        Matrix<int> x;
        x = w;
        qDebug() << "x = w : " << x.toString();
    }


private:
    unsigned int m_height; ///< La hauteur de la Matrix.
    unsigned int m_width; ///< La largeur de la Matrix.
    Vector<T> m_vector; ///< Vector stockant les données de la Matrix.
};

#endif // MATRIX_H
