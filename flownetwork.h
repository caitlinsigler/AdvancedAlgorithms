//
//  flownetwork.h
//  Header file for a flow network
//
//  Created by Caitlin Sigler on 3/11/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef flownetwork_h
#define flownetwork_h

#include "network.h"
#include <queue>
#include <unordered_map>

template <class T>
class flow: public network<T>
{
  public:

    flow(const T & source, const T & sink)
    {
        _source = source;
        _sink = sink;
        digraph<T>::add_vertex(_source);
        digraph<T>::add_vertex(_sink);
    }


    void add_vertex(const T & v)
    {
        if (v == _source || v == _sink)
            return;

        digraph<T>::add_vertex(v);
    }
    bool empty() const
    {
        return (digraph<T>::m() == 0);
    }

    double value() const
    {
        double ans(0.0);
        for (auto n: digraph<T>::Adj(_source))
            ans += network<T>::cost(_source, n);

        return ans;
    }

    void operator +=(const flow& f)
    {
        for (auto e: f.E())
        {
            if (digraph<T>::isEdge(e.s, e.d))
                network<T>::increase_cost(e.s, e.d, f.cost(e.s, e.d));
            else
                network<T>::add_edge(e.s, e.d, f.cost(e.s, e.d));
        }
    }

private:

    T _source, _sink;

};


template <class T>
class flow_network: public network<T>
{
public:

    flow_network(const T & source, const T & sink)
    {
        _source = source;
        _sink = sink;
        digraph<T>::add_vertex(_source);
        digraph<T>::add_vertex(_sink);
    }

    void add_vertex(const T & v)
    {
        if (v == _source || v == _sink)
            return;
        digraph<T>::add_vertex(v);
    }

    T source() const
    {
        return _source;

    }

    T sink () const
    {
        return _sink;
    }


    flow<T> augmented_flow()
    {
        // find a path of fewest edges from source to sink using BFS
        // construct flow from this path with value = min of capacities on this path
        // compute residual network


        std::queue<T> q;
        std::unordered_map<T, T> parent;

        q.push(_source);
        parent[_source] = _source;

        while (!q.empty())
        {
            T front = q.front();
            q.pop();

            for (auto n: digraph<T>::Adj(front))
            {
                if (parent.count(n) == 0) // unvisited
                {
                    parent[n] = front;
                    q.push(n);
                    if (n == _sink)
                        break;
                }
            }
        }

        if (parent.count(_sink) == 0)  // no more augmenting path
            return flow<T>(_source, _sink);

        double w(std::numeric_limits<double>::infinity());
        for (T v = _sink; v != _source; v = parent[v])
            w = std::min(w, network<T>::cost(parent[v], v));

        flow<T> ans(_source, _sink);
        for (auto v: digraph<T>::V())
                ans.add_vertex(v);

        for (T v = _sink; v != _source; v = parent[v])
        {
            // add to flow
            ans.add_edge(parent[v], v, w);


            // update this flow network
            network<T>::increase_cost(parent[v], v, -w);
            if (network<T>::cost(parent[v], v) == 0.0)
                network<T>::remove_edge(parent[v], v);

            if (digraph<T>::isEdge(v, parent[v]))
                network<T>::increase_cost(v, parent[v], w);
            else
                network<T>::add_edge(v, parent[v], w);


        }


    return ans;


 }


    //calculate max flow
    flow<T> max_flow() const
    {
        flow_network residual(*this);

        flow<T> ans(_source, _sink);

        for (auto v: digraph<T>::V())
            if (v != _source && v!= _sink)
                ans.add_vertex(v);

        flow<T> f = ans;

        do
        {
            f = residual.augmented_flow();
            ans += f;
          } while (!f.empty());

        return ans;
    }


private:

    T _source, _sink;

};


// output operator for network is the same as for flow_network

template <class T>
std::istream & operator >>(std::istream & is, flow_network<T> & N)
{
    std::size_t n, m;
    T s, d;
    double w;

    // assume that the first two vertices are source and sink

    is >> n >> m;
    assert(n >= 2);

    is >> s >> d;   // first two vertices are source and sink

    flow_network<T> temp(s, d);
    for (std::size_t i = 2; i < n; ++i)
    {
        is >> s;
        temp.add_vertex(s);
    }

    for (std::size_t i = 0; i < m; ++i)
    {
        is >> s >> d >> w;

        temp.add_edge(s, d, w);
    }

    N = temp;
    return is;


}

#endif /* flownetwork_h */
