//
//  network.h
//  Header file for a network
//
//  Created by Caitlin Sigler on 3/11/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef network_h
#define network_h
#include "wedge.h"
#include "digraph.h"
#include <set>
#include "dary_heap.h"
template <class T>
class network: public digraph<T>
{
public:

    network()
    {

    }

    void add_edge(const T & s, const T & d, double w)
    {
        digraph<T>::add_edge(s, d);
        _w[{s, d}] = w;
    }

    void add_edge(const WEdge<T> & e)
    {
        add_edge(e.s, e.d, e.w);
    }

    double cost(const T & s, const T & d) const
    {
        assert(digraph<T>::isVertex(s) && digraph<T>::isVertex(d));
        return _w.at({s, d});
    }

    std::set<WEdge<T>> E() const
    {
        std::set<WEdge<T>> ans;
        for (auto v: digraph<T>::V())
            for (auto w: digraph<T>::Adj(v))
                ans.insert(WEdge<T>(v, w, cost(v, w)));

        return ans;
    }


    network Dijkstra(const T & s) const
    {
        network ans;
        dary_heap<WEdge<T>> H;
        std::unordered_map<T, WEdge<T>> best;

        for (auto v: digraph<T>::V())
            ans.add_vertex(v);

        std::unordered_set<T> out;  // out of heap, inside special vertex

        best[s] = {s, s, 0};    //  dist(s, s) = 0
        T v = s;       // vertex whose true distance was just found; initially v = s, given start vertex
        out.insert(s);

        do
        {
            for (auto n: digraph<T>::Adj(v))
            {
                if (out.count(n) != 0)
                    continue;

                WEdge<T> fringe({v, n, best[v].w+cost(v,n)});

                if (best.count(n) == 0)
                {
                    H.push(fringe);
                    best[n] = fringe;
                }
                else if (best[n].w > fringe.w)
                {
                    best[n] = fringe;
                    H.decrease_key(best[n], fringe);
                }
            }

            WEdge<T> m = H.min();
            H.pop_min();
            ans.add_edge(m.s, m.d, cost(m.s, m.d));
            v = m.d;
            out.insert(v);
        } while (!H.empty());




    return ans;
    }


    std::unordered_map<T, T> Bellman_Ford(const T & s, std::unordered_map<T, double> &d)
    {
        std::unordered_map<T, T> parent;  // (parent(v), v) is last edge on shortest path from s to v
        std::unordered_map<T, double> D;  // shortest distance from s to  v

        for (auto v: digraph<T>::V())
            d[v] = std::numeric_limits<double>::infinity();

        d[s] = 0;

        for (std::size_t k = 1; k < digraph<T>::n(); ++k)
        {
            D = d;
            //for (auto e: digraph<T>::E())
            for (auto v: digraph<T>::V())
            {
                for (auto n: digraph<T>::Adj(v))
                {
                    double temp = d[v] + cost(v, n);
                    if (D[n] > temp)  // found better route
                    {
                        D[n] = temp;
                        parent[n] = v;
                    }
                }
            }
            d = D;
        }

        network<T> ans;
        for (auto v: digraph<T>::V())
            ans.add_vertex(v);

        for (auto e: parent)
            ans.add_edge(e.second, e.first, cost(e.second, e.first));
        
        return parent;
    }
    
    std::vector<int> Bellman_Ford_2(const T & s)
    {
        std::unordered_map<T, T> parent;  // (parent(v), v) is last edge on shortest path from s to v
        std::unordered_map<T, double> d, D;  // shortest distance from s to  v
        for (auto v: digraph<T>::V())
            d[v] = std::numeric_limits<double>::infinity();

            d[s] = 0;

           for (std::size_t k = 1; k < digraph<T>::n(); ++k)
           {
               D = d;
               for (auto v: digraph<T>::V())
               {
                   for (auto n: digraph<T>::Adj(v))
                   {
                       double temp = d[v] + cost(v, n);
                       if (D[n] > temp)  // found better route
                       {
                           D[n] = temp;
                           parent[n] = v;
                       }
                   }
               }
               d = D;
           }
           
           int changed_vertex=0;
           int changed_parent=0;
           std::vector<int> path;
           //perform one extra loop and check for changes indicating a negative cycle
           for (auto v: digraph<T>::V())
           {
               for (auto n: digraph<T>::Adj(v))
               {
                   double temp = d[v] + cost(v, n);
                   if (D[n] > temp)  // found better route
                   {
                       //something happened--negative path
                       changed_vertex=n;
                       changed_parent= v;
                       //create path from vertex to vertex parent
                       while(changed_parent != changed_vertex){
                            path.push_back(changed_parent);
                            changed_parent= parent[changed_parent];
                       }
                       path.push_back(changed_vertex);
                       std::reverse(path.begin(),path.end());
                       
                       return path;
                   }
               }
           }
           
        return std::vector<int> ();
       }

private:

    std::unordered_map<Edge<T>, double> _w;  // maps an edge to its weight
};

template <class T>
std::ostream & operator << (std::ostream & os, const network<T> & N)
{
    os << N.n() << " " << N.m() << std::endl;
    for (auto v: N.V())
        os << v << " ";
    os << std::endl;

    for (auto v: N.V())
        for (auto w: N.Adj(v))
            os << v << " " << w << " " << N.cost(v, w) << std::endl;

    return os;
}

template <class T>
std::istream & operator >> (std::istream & is, network<T> & N)
{
    std::size_t n, m;
    T s, d;
    double w;

    is >> n >> m;

    for (std::size_t i = 0; i < n; ++i)
    {
        is >> s;
        N.add_vertex(s);
    }

    for (std::size_t i = 0; i < m; ++i)
    {
        is >> s >> d >> w;
        N.add_edge(s, d, w);
    }

    return is;
}

#endif /* network_h */
