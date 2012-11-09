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

#endif // COLLECTION_H
