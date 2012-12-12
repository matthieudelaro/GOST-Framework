#ifndef COLLECTION_H
#define COLLECTION_H

/*! \brief La structure Pair permet de gérer deux éléments (fist et second) de types quelconques en même temps.
 */
template <typename T, typename U>
struct Pair
{
    T first;///< Le premier élément.
    U second;///< Le deuxième élément.
};

/*! \brief Permet de comparer deux instances de Pair.
 *  \return true si les éléments sont égaux deux à deux, false sinon.
 */
template <typename T, typename U>
bool operator==(const Pair<T, U> &p1, const Pair<T, U> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second);
}

/*! \brief La structure Triple permet de gérer trois éléments (fist, second and third) de types quelconques en même temps.
 */
template <typename T, typename U, typename V>
struct Triple
{
    T first;///< Le premier élément.
    U second;///< Le deuxième élément.
    V third;///< Le troisième élément.
};

/*! \brief Permet de comparer deux instances de Triple.
 *  \return true si les éléments sont égaux deux à deux, false sinon.
 */
template <typename T, typename U, typename V>
bool operator==(const Triple<T, U, V> &p1, const Triple<T, U, V> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second) && (p1.third == p2.third);
}

/*! \brief La structure Quadruple permet de gérer quatres éléments (fist, second, third and fourth) de types quelconques en même temps.
 */
template <typename T, typename U, typename V, typename W>
struct Quadruple
{
    T first;///< Le premier élément.
    U second;///< Le deuxième élément.
    V third;///< Le troisième élément.
    W fourth;///< Le quatrième élément.
};

/*! \brief Permet de comparer deux instances de Quadruple.
 *  \return true si les éléments sont égaux deux à deux, false sinon.
 */
template <typename T, typename U, typename V, typename W>
bool operator==(const Quadruple<T, U, V, W> &p1, const Quadruple<T, U, V, W> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second) && (p1.third == p2.third) && (p1.fourth == p2.fourth) ;
}

#endif // COLLECTION_H
