//
//  edge.h
//  Header file for the edge of a graph
//
//  Created by Caitlin Sigler on 3/11/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef edge_h
#define edge_h

#include <iostream>

template <class T>
struct Edge
{
    T s, d;

    Edge(const T & s = T(),
         const T & d = T()): s(s), d(d)
    {}
};

template <class T>
std::ostream & operator << (std::ostream & os, const Edge<T> & e)
{
    os << "(" << e.s << ", " << e.d << ")";
    return os;
}


template <class T>
bool operator == (const Edge<T> & e1, const Edge<T> & e2)
{
    return (e1.s == e2.s && e1.d == e2.d);
}

namespace std
{
    template <class T>
    class hash<Edge<T>>
    {
    public:
        std::size_t operator() (const Edge<T> & e) const
        {
            return std::hash<T>() (e.s) ^
                    std::hash<T>() (e.d);
        }
    };
}


#endif /* edge_h */
