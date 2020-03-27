//
//  ph.h
//  Header file for a pairing heap
//
//  Created by Caitlin Sigler on 3/4/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#ifndef ph_h
#define ph_h
#include <vector>
#include <unordered_map>

template <class T>
class ph
{
public:
    struct node
    {
        T key;
        node * parent;  // points to parent
        node * child;  // points to first child
        node * next;   // points to next sibling
        node * prev;   // points to previous sibling

        node(const T & k = T(),
             node * P = nullptr,
             node * c = nullptr,
             node * n = nullptr,
             node * p = nullptr): key(k), parent(P), child(c), next(n), prev(p)
        {
        }
    };
    //default constructor
    ph(){
        
    }
    
    //destructor
    ~ph(){
        free(head);
    }
    
    //post: returns true iff heap is empty
    bool empty() const{
        return (head== nullptr);
    }
    
    //post: returns the minimum key in this heap
    T min() const{
        return (head->key);
    }
    
    //pre: key does not already exist in heap
    //post: inserts a new key into this heap
    void push(const T & key){
        assert(_l.count(key)==0);
        
        node *p1 = new node (key);
        //add node to _l
        _l[key]=p1;
        //merge with heap
        head= merge(head,p1);
    }
    
    //post: removes the minimum key of this heap
    void pop_min(){
        //check for children
        if (head->child!=nullptr){
            T key;
            std::vector<node *> v;
            //obtain first child node
            node *next_child= head->child;
            v.push_back(next_child);
            //delete memory in head
            free(head);
            //store children in v
            while(next_child->next!=nullptr){
                v.push_back(next_child->next);
                next_child= next_child->next;
            }
            //first pass: merge consecutive pairs into sub heaps
            std::vector<node *> v2;
            for(int i=0; i<v.size(); i+=2){
                v2.push_back(merge(v[i],v[i+1]));
            }
            //second pass: merge each subheap
            head=merge(v2[0], v2[1]);
            for(int j=2; j<v2.size(); j++){
                head=merge(head, v2[j]);
            }
        }
        //if min has no children, free the memory in root
        else{
            free(head);
        }
    }
    
    
    
    //pre: newx is not in heap
    //post: decreases key oldx to a smaller value newx
    void decrease_key(const T & oldx, const T & newx){
        assert(_l.count(newx)==0);
        node *p1= _l[oldx];
        //check if node has a parent
        if(p1->parent!=nullptr){
            //check if parent is > or < new key value
            if(newx < p1->parent->key){
                //check if node has sibling to the left
                if(p1->prev!=nullptr){
                    //reset prev
                    node *p1_next= p1->next;
                    p1_next->prev= p1->prev;
                }
                //check if node has sibling to the right
                if(p1->next!=nullptr){
                    //reset next
                    node *p1_prev= p1->prev;
                    p1_prev->next= p1->next;
                    
                }
                //reset key and merge node with root;
                p1->key=newx;
                head= merge(head, p1);
            }
        }
        else{//either is root (no parent) or new value is not < parent
            p1->key=newx;
        }
    }


private:
    node * head;               // points to the root
    std::unordered_map<T, node *> _l;  //maps key to node containing it
    
    
    //merges two pairing heaps
    node * merge(node *p1, node *p2){
        if (p1==nullptr){
            return p2;
        }
        if(p2==nullptr){
            return p1;
        }
        if(p1->key < p2->key){
            //p1 is parent
            //reset next
            p2->next= p1->child;
            //reset previous, if it exists
            if(p1->child != nullptr){
                (p1->child)->prev= p2;
            }
            //reset child
            p1->child=p2;
            //reset parent
            p2->parent= p1;
            return p1;
        }
        else{
            //make p2 parent
            p1->next= p1->child;
            p2->child= p1;
            return p2;
        }
    }
};

#endif /* ph_h */
