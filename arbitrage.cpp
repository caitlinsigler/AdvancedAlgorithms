//
//  arbitrage.cpp
//  This file takes in an input file of converstion rates of various currencies, and calculates if there is a possible arbitrage
//
//  Created by Caitlin Sigler on 3/12/20.
//  Copyright © 2020 Caitlin Sigler. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include "wedge.h"
#include "digraph.h"
#include "dary_heap.h"
#include "network.h"
#include "flownetwork.h"
#include "edge.h"

using namespace std;

template <class T>

//pre: inpute file of exchange rates is in matrix format
//post: prints arbitrage path if possible, else prints "no possible arbitrage"
int arbitrage(){
    //get input
    std:: ifstream is("/Users/caitlinsigler16/Desktop/input3.txt");
    vector<int> ans;
    int n;
    //number of currencies
    is>>n;
       
    //add vertices and weights from input to network
    network<T> net;
    for(int k=0; k<n; k++){
        net.add_vertex(k);
    }
    double y;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            is>>y;
            if(i!=j){
                y=(-1)*log(y);
                net.add_edge(i, j, y);
            }
        }
    }
    
    //call Bellman Ford algorithm
    vector<int> path=net.Bellman_Ford_2(0);
     
    //print result
    if (path.size()==0){
        cout<<"no possible arbitage"<<endl;
    }
    else{
        for(auto e: path){
            cout<<e<<" ";
        }
        cout<<endl;
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    arbitrage<int>();
    return 0;
}
