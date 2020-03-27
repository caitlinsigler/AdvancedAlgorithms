//
//  dary_heap.h
//  Header file for a d-ary heap
//
//  Created by Caitlin Sigler on 3/11/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef dary_heap_h
#define dary_heap_h

#include <vector>
#include <unordered_map>
#include <cassert>

template <class T>
class dary_heap
{

public:

    dary_heap(std::size_t d = 2): _d(d)
    {
        _n = 0;
    }

    bool empty() const
    {
        return (_n == 0);
    }

    T min() const
    {
        assert(!empty());
        return _data[0];
    }

    void push(const T & x)
    {
        assert(_l.count(x) == 0);

        // place new element at the end of array

        if (_n == _data.size())   // out of space
            _data.push_back(x);
        else
            _data[_n] = x;

        _l[x] = _n;
        ++_n;

        // move x up heap if necessary

        std::size_t i = _l[x];
        while (i > 0)
        {
            std::size_t parent = (i-1)/_d;
            if (_data[i] < _data[parent])
            {
                std::swap(_data[i], _data[parent]);
                _l[_data[i]] = i;
                _l[_data[parent]] = parent;
                i = parent;
            }
            else
                break;
        }

    }


    void decrease_key(const T & x, const T & newx)
    {
        assert(newx < x && _l.count(x) != 0 && _l.count(newx) == 0);

        std::size_t i = _l[x];
        _data[i] = newx;
        _l.erase(x);
        _l[newx] = i;

        while (i > 0 && _data[i] < _data[(i-1)/_d])
        {
            std::size_t parent((i-1)/_d);
            std::swap(_data[i], _data[parent]);
            _l[_data[i]] = i;
            _l[_data[parent]] = parent;
            i = parent;
        }

    }

    //deletes the root
    void pop_min()
    {
        assert(!empty());

        _l.erase(_data[0]);
        if (--_n == 0)
            return;

        _data[0] = _data[_n];
        _l[_data[0]] = 0;
        std::size_t i = 0;

        while (i*_d +1 < _n)  // not a leaf yet because leftmost child exists
        {
            std::size_t left(i*_d + 1), right((i+1)*_d), m(left); // m points to smallest child
            for (std::size_t c = left+1; c <= right && c < _n; ++c)
            {
                if (_data[c] < _data[m])
                    m = c;
            }

            if (!(_data[m] < _data[i]))
                break;

            std::swap(_data[i], _data[m]);
            _l[_data[i]] = i;
            _l[_data[m]] = m;
            i = m;
        }
    }



private:
    std::vector<T> _data;      // store heap elements
    std::size_t    _n;         // actual number of heap elements
    std::size_t    _d;         // number of children per node
    std::unordered_map <T, std::size_t>  _l;    // _data[_l[key]] = key
};

#endif /* dary_heap_h */
