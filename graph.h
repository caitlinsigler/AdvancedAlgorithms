//
//  graph.h
//  Header file for a graph
//
//  Created by Caitlin Sigler on 3/11/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef graph_h
#define graph_h

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cassert>
#include <utility>
#include <queue>
#include <stack>
#include <vector>


#ifndef HASH_PAIR_OF_STRINGS
#define HASH_PAIR_OF_STRINGS

namespace std
{
    template<>
        class hash<std::pair<std::string, std::string>>
        {
        public:
            std::size_t operator () (const std::pair<std::string, std::string> & p) const
            {
                return std::hash<std::string> () (p.first) ^
                       std::hash<std::string> () (p.second);
            }
        };
}
#endif

class graph
{
public:

    typedef std::string Vertex;   // vertices are strings
    typedef std::unordered_set<Vertex> VertexSet;
    typedef std::vector<Vertex> Path;

    typedef std::unordered_map<Vertex, Vertex> V2V;       // function that maps vertex to vertex
    typedef std::unordered_map<Vertex, std::size_t> V2I;  // function that maps vertex to a count

    typedef std::pair<Vertex, Vertex> Edge;
    typedef std::unordered_set<Edge> EdgeSet;

    // default constructor
    graph()
    {

    }

    // constant member functions

    // pre: none
    // post: returns the number of vertices
    std::size_t n() const
    {
        return _t.size();
    }

    // pre: none
    // post: returns the number of edges
    std::size_t m() const
    {
        std::size_t ans(0);

        for (auto p: _t)
            ans += p.second.size();

        return ans / 2;

    }

    // pre: none
    // post: returns the set of vertices

    VertexSet V() const
    {
        VertexSet ans;

        for (auto p: _t)
            ans.insert(p.first);

        return ans;
    }

    // pre: none
    // post: returns the set of edges
    /* EdgeSet E() const
    {

    }
    */

    // pre: none
    // post: returns true iff v is a vertex

    bool isVertex(const Vertex & v) const
    {
        return (_t.count(v) == 1);
    }

    // pre: v is a vertex
    // post: returns the degree of v

    std::size_t deg(const Vertex & v) const
    {
        assert(isVertex(v));
        return _t.at(v).size();  // _t[v].size()
    }

    // pre: v is a vertex
    // post: returns the set of vertices adjacent to v

    VertexSet Adj(const Vertex &v) const
    {
        assert(isVertex(v));
        return _t.at(v);

    }

    V2V bfs(const Vertex & source) const
    {
        assert(isVertex(source));

        std::queue<Vertex> Q;
        V2V parent;

        // initialization
        Q.push(source);
        parent[source] = source;

        // loop
        while (!Q.empty())
        {
            Vertex front = Q.front();
            Q.pop();

            for (auto w: Adj(front))
            {
                if (parent.count(w) == 0) // w is not visited
                {
                    Q.push(w);
                    parent[w] = front;
                }
            }

        }

        return parent;
    }


    V2V idfs(const Vertex & source, std::size_t time)
    {
        assert(isVertex(source));

        std::stack<Vertex> S;

        V2V parent;
        std::unordered_map<Vertex, std::size_t> pre;  // preorder number of each vertex

        // init
        S.push(source);

        // loop

        while (!S.empty())
        {
            Vertex top = S.top();

            if (pre.count(top) == 0) // not visited
            {
                pre[top] = time++;
                for (auto w: Adj(top))
                {
                    if (pre.count(w) == 0)
                    {
                        S.push(w);
                        parent[w] = top;
                    }
                }
            }
            else // already visited
                S.pop();



        }

        return parent;
    }

    void dfs(const Vertex & source,
             std::size_t & time,
             V2I & pre,
             V2I & post,
             V2I & low,
             V2V & tree,   // tree[child] = parent
             V2V & back)   // back[descendant] = ancestor
    {
        assert(isVertex(source));

        pre[source] = time++;
        low[source] = pre[source];
        for (auto w: Adj(source))
        {
            if (pre.count(w) == 0) // unvisited neighbor so (source, w) is a tree edge
            {
                tree[w] = source;
                dfs(w, time, pre, post, low, tree, back);
                low[source] = std::min(low[source], low[w]);


            }
            else if (tree.count(source) == 0 || tree[source] != w)
            {
                    // (source, w) is a back edge
                back[source] = w;
                low[source] = std::min(low[source], pre[w]);
            }

        }

        post[source] = time++;

    }

    // pre: none
    // post: returns the number of connnected components
    std::size_t c() const
    {
        std::size_t ans(0);
        VertexSet visited;

        for (auto v: V())
        {
            if (visited.count(v) == 0)   // has not been visited
            {
                ++ans;
                V2V parent = bfs(v);
                for (auto p: parent)
                    visited.insert(p.first);
            }
        }

        return ans;
    }

    bool isConnected() const
    {
        return (c() == 1);
    }

    bool isAcyclic() const
    {
        return (n() == m() + c());
    }

    bool isTree() const
    {
        return isConnected() && isAcyclic();
    }

    bool isBipartite() const
    {
        V2I color;

        for (auto v: V())
        {
            if (color.count(v) == 0)
            {
                std::queue<Vertex> Q;
                Q.push(v);
                color[v] = 0;

                while (!Q.empty())
                {
                    Vertex front = Q.front();
                    Q.pop();

                    for (auto w: Adj(front))
                    {
                        if (color.count(w) == 0)
                        {
                            Q.push(w);
                            color[w] = 1 - color[front];
                        }
                        else
                        {
                            if (color[w] == color[front])
                                return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    bool isComplete() const
    {
        std::size_t nv = n();
        return (2*m() == nv*(nv-1));
    }

    bool isEulerian() const
    {
        for (auto v: V())
            if (deg(v) % 2 != 0)
                return false;
        return isConnected();
    }

    Path eulerianCycle() const
    {
        assert(isEulerian());
        Path ans;

        graph copy(*this);  // make copy of this graph

        std::stack<Vertex> S;

        Vertex v = *(copy.V().begin());  // v is the first vertex in the set of vertices of copy
        S.push(v);
        std::cout << "start vertex: " << v << std::endl;

        while (!S.empty())
        {
            Vertex s = S.top();
            if (copy.deg(s) != 0)
            {
                Vertex w = *(copy.Adj(s).begin());  // w is the first neighbors in the set of neighbors of s;
                std::cout << "neighbor: " << w << std::endl;
                copy.remove_edge(s, w);

                S.push(w);
            }
            else   // s has no more neighbors
            {
                S.pop();
                ans.push_back(s);
            }
        }

        return ans;
    }



    // mutator member functions

    // pre: none
    // post: returns false if v is already a vertex; otherwise adds vertex v and returns true
    bool add_vertex(const Vertex & v)
    {
        if (isVertex(v))
            return false;

        _t[v] = VertexSet();
        return true;
    }

    // pre: v and w are different vertices
    // post: adds edge {v, w} if it doesn't exist; otherwise do nothing

    void add_edge(const Vertex &v, const Vertex &w)
    {
        assert(isVertex(v) && isVertex(w) && v != w);
        _t[v].insert(w);
        _t[w].insert(v);
    }

    // pre: v and w are vertices
    // post: remove edge {v, w} if it exists; otherwise do nothing
    void remove_edge(const Vertex &v, const Vertex &w)
    {
        assert(isVertex(v) && isVertex(w));
        _t[v].erase(w);
        _t[w].erase(v);
    }

private:

    std::unordered_map<Vertex, VertexSet> _t;  // Adjacency "hashmap" representation

};

std::ostream & operator << (std::ostream & os, const graph & G)
{
    os << G.n() << " " << G.m() << std::endl;

    for (auto v: G.V())
        os << v << " ";
    os << std::endl;

    for (auto v: G.V())
        for (auto w: G.Adj(v))
            if (v < w)
                os << v << " " << w << std::endl;

    return os;
}

std::istream & operator >> (std::istream &is, graph & G)
{
   std::size_t n, m;
   std::string v, w;

   is >> n >> m;

   for (std::size_t i = 1; i <= n; ++i)
   {
       is >> v;
       G.add_vertex(v);
   }

   for (std::size_t i = 1; i <= m; ++i)
   {
       is >> v >> w;
       G.add_edge(v, w);
   }

   return is;
}

#endif /* graph_h */
