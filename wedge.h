//
//  wedge.h
//  Header file for a weighted edge
//
//  Created by Caitlin Sigler on 3/11/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef wedge_h
#define wedge_h

#include "edge.h"

template <class T>
struct WEdge: public Edge<T>
{
    double w;

    WEdge(const T & s = T(),
          const T & d = T(),
          double w = 0.0): Edge<T>(s, d), w(w)
    {

    }
};

template <class T>
std::ostream & operator << (std::ostream & os, const WEdge<T> & e)
{
    os << "[" << e.s << ", " << e.d << ", " << e.w << "]";
    return os;
}

template <class T>
bool operator < (const WEdge<T> & e1, const WEdge<T> & e2)
{
    return (e1.w < e2.w) || (e1.w == e2.w && e1.s < e2.s) ||
            (e1.w == e2.w && e1.s == e2.s && e1.d < e2.d);
}

template <class T>
bool operator == (const WEdge<T> & e1, const WEdge<T> & e2)
{
    return (e1.s == e2.s && e1.d == e2.d && e1.w == e2.w);
}


namespace std
{
    template <class T>
    class hash<WEdge<T>>
    {
        std::size_t operator() (const WEdge<T> & e) const
        {
            return std::hash<Edge<T>>() (e) ^
                    std::hash<double>() (e.w);
        }
    };
}

#endif /* wedge_h */
