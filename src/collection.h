#ifndef COLLECTION_H
#define COLLECTION_H

template <typename T, typename U>
struct Pair
{
    T first;
    U second;
};

template <typename T, typename U>
bool operator==(const Pair<T, U> &p1, const Pair<T, U> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second);
}

template <typename T, typename U, typename V>
struct Triple
{
    T first;
    U second;
    V third;
};

template <typename T, typename U, typename V>
bool operator==(const Triple<T, U, V> &p1, const Triple<T, U, V> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second) && (p1.third == p2.third);
}

template <typename T, typename U, typename V, typename W>
struct Quadruple
{
    T first;
    U second;
    V third;
    W fourth;
};

template <typename T, typename U, typename V, typename W>
bool operator==(const Quadruple<T, U, V, W> &p1, const Quadruple<T, U, V, W> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second) && (p1.third == p2.third) && (p1.fourth == p2.fourth) ;
}

/* Version avec des class, mais qui n'a aucun intéret pour l'instant
template <typename T, typename U>
class Pair
{
public:
    T first;
    U second;
};

template <typename T, typename U>
inline bool operator==(const Pair<T, U> &p1, const Pair<T, U> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second);
}

template <typename T, typename U, typename V>
class Triple : public Pair<T, U>
{
public:
    V third;
};

template <typename T, typename U, typename V>
bool operator==(const Triple<T, U, V> &p1, const Triple<T, U, V> &p2)
{
    return (p1.first == p2.first) && (p1.second == p2.second) && (p1.third == p2.third);

    //manière générique mais plus lente qu'utile
    //return operator==(Pair<T, U>(p1), Pair<T, U>(p2)) && (p1.third == p2.third);
}*/

#endif // COLLECTION_H
