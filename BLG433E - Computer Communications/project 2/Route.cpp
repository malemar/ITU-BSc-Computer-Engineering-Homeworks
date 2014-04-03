/* 
 * File:   Route.cpp
 * Author: Volkan
 * 
 */


#include "Route.h"
#include <iostream>

// Constructors & Destructors

Route::Route() {
}

Route::Route(const Route& orig) {
}

Route::~Route() {
}

Route::Route(const Node* n1, const Node* n2) : node1(n1), node2(n2){
    
}

// Member Functions
void Route::addNode(const Node* n){
    path.push_back(n);
}

void Route::printPath() const {
    for(std::list<const Node*>::const_iterator it = path.begin() ; it != path.end()
            ; ++it){
        std::cout << (*it)->getId();
        if(it != path.end()){
            std::list<const Node*>::const_iterator ite = it;
            ite++;
            if(ite != path.end()) std::cout << "-->";
        }
    }
}