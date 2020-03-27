//
//  digraph.h
//  Header file for a digraph
//
//  Created by Caitlin Sigler on 3/11/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef digraph_h
#define digraph_h


#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <utility>
#include <list>
#include <stack>

template <class Vertex>
class digraph
{
public:

    //typedef std::string Vertex;
    typedef std::pair<Vertex, Vertex> DEdge;
    typedef std::vector<Vertex> DPath;

    typedef std::unordered_set<Vertex> VertexSet;
    typedef std::unordered_map<Vertex, Vertex> V2V;
    typedef std::unordered_map<Vertex, int> V2I;


    // constructor

    digraph()
    {

    }

    std::size_t n() const
    {
        return _t.size();
    }

    std::size_t m() const
    {
        std::size_t ans(0);
        for (auto p: _t)
            ans += p.second.size();

        return ans;
    }

    bool isVertex(const Vertex & v) const
    {
        return (_t.count(v) != 0);
    }

    bool isEdge(const Vertex &s, const Vertex & d) const
    {
        assert(isVertex(s) && isVertex(d));
        return (Adj(s).count(d) != 0);
    }

    std::size_t outdeg(const Vertex & v) const
    {
        assert(isVertex(v));
        return _t.at(v).size();
    }

    std::size_t indeg(const Vertex &v) const
    {
        std::size_t ans(0);

        assert(isVertex(v));
        for (auto p: _t)
            ans += p.second.count(v);

        return ans;
    }

    VertexSet V() const
    {
        VertexSet ans;
        for (auto p: _t)
            ans.insert(p.first);

        return ans;
    }

    //returns set of all adjacent vertices
    VertexSet Adj(const Vertex & v) const
    {
        assert(isVertex(v));
        return _t.at(v);
    }

    digraph reverse() const
    {
        digraph R;
        for (auto v: V())
            R.add_vertex(v);

        for (auto v: V())
            for (auto w: Adj(v))
                R.add_edge(w, v);

        return R;
    }

    //DFS for Kosaraju's connected components algorithm
    void Kdfs(const Vertex & v,
              V2I & visited,
              std::list<Vertex> & lv) const
    {
        visited[v] = 1;
        for (auto w: Adj(v))
        {
            if (visited.count(w) == 0)
                Kdfs(w, visited, lv);

        }
        lv.push_front(v);

    }
    
    //Kosaraju's connected components algorithm
    V2I Kscc() const
    {
        V2I visited, ans;
        digraph R = reverse();
        std::list<Vertex> lv;

        // dfs on reverse graph to obtain list of vertices sorted in
        // reverse post order number
        for (auto v: R.V())
            if (visited.count(v) == 0)
                R.Kdfs(v, visited, lv);

        visited.clear();
        std::size_t name(1);
        for (auto v: lv)
        {
            std::list<Vertex> component;
            if (visited.count(v) == 0)
            {
                Kdfs(v, visited, component);
                for (auto w: component)
                    ans[w] = name;
                ++name;
            }

        }

        return ans;
    }

    //DFS for Tarjan's connected component algorithm
    void Tdfs(const Vertex & v,
              V2I & pre,
              V2I & low,
              V2I & ans,
              std::size_t &time,
              std::size_t &name,
              std::stack<Vertex> & S
              ) const
    {
        pre[v] = low[v] = time++;
        S.push(v);

        for (auto w: Adj(v))
        {
            if (pre.count(w) == 0)
                Tdfs(w, pre, low, ans, time, name, S);
            low[v] = std::min(low[v], low[w]);   // no need to worry about cross edge
                                                 // because processed connected components
                                                 // have low values set to n
        }


        if (low[v] == pre[v])
        {
            Vertex top;
            std::size_t _n = n();
            do
            {
                top = S.top();
                ans[top] = name;
                S.pop();
                low[top] = _n;   // disable cross edges
            } while (top != v);
            ++name;

        }
    }

    //Tarjan's connected component algorithm
    V2I Tscc() const
    {

        V2I pre, low, ans;
        std::size_t time(1), name(1);
        std::stack<Vertex> S;

        for (auto v: V())
        {
            if (pre.count(v) == 0)
                Tdfs(v, pre, low, ans, time, name, S);
        }
        return ans;


    }

    // mutator functions

    void add_vertex(const Vertex & v)
    {
        assert(!isVertex(v));

        _t[v] = VertexSet();
    }

    void add_edge(const Vertex & s, const Vertex & d)
    {
        assert(isVertex(s) && isVertex(d));
        _t[s].insert(d);
    }

    void remove_edge(const Vertex &s, const Vertex &d)
    {
        assert(isVertex(s) && isVertex(d));
        _t[s].erase(d);
    }




private:

    std::unordered_map<Vertex, VertexSet> _t;
};


template <class T>
std::ostream & operator << (std::ostream & os, const digraph<T> & D)
{
    os << D.n() << " " << D.m() << std::endl;
    for (auto v: D.V())
        os << v << " ";
    os << std::endl;

    for (auto v: D.V())
        for (auto w: D.Adj(v))
            os << v << " " << w << std::endl;
    return os;
}

template <class T>
std::istream & operator >> (std::istream & is, digraph<T> & D)
{
    std::size_t n, m;
    is >> n >> m;
    std::string s, d;
    D = digraph<T>();
    for (std::size_t i = 1; i <= n; ++i)
    {
        is >> s;
        D.add_vertex(s);
    }
    for (std::size_t i = 1; i <= m; ++i)
    {
        is >> s >> d;
        D.add_edge(s, d);
    }
    return is;
}

#endif /* digraph_h */
