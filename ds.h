//
//  ds.h
//  Header File for a disjoint set
//
//  Created by Caitlin Sigler on 3/12/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef ds_h
#define ds_h

#include <unordered_map>
template <class T>
class ds
{
public:

    struct node
    {
        T value;
        std::size_t rank;
        node * parent;

        node(const T & x)
        {
            value = x;
            rank = 0;
            parent = this;
        }

    };
    ds()
    {

    }
    ~ds()
    {
        for (auto e: _data)
            delete e.second;
    }

    void make_set(const T & x)
    {
        assert(_data.count(x) == 0);  // x is not yet in the collection
        _data[x] = new node(x);
    }

    node *  find(const T & x)
    {
        assert(_data.count(x) != 0);  // x must be in the collection

        if (_data[x] == _data[x]->parent)
            return _data[x];

        //  return find(_data[x]->parent);   if not using path compression

        return (_data[x]->parent = find(_data[x]->parent->value)); // with path compression

    }

    // pre: x and y are in the collection
    // post: returns true if two trees are combined; false otherwise
    bool join(const T & x, const T & y)
    {
        assert(_data.count(x) != 0 && _data.count(y) != 0);
        node * rx = find(x), * ry = find(y);

        if (rx == ry)           // x and y share the same root
            return false;

        if (rx->rank > ry->rank)
            ry->parent = rx;
        else if (rx-> rank < ry->rank)
            rx->parent = ry;
        else
        {
            ry->parent = rx;
            rx->rank++;
        }

        return true;


    }


private:
    std::unordered_map<T, node*> _data;
};

#endif /* ds_h */
