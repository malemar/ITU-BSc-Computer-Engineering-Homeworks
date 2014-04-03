/* 
 * File:   Node.cpp
 * Author: Volkan
 * 
 * Created on 29 Kasım 2013 Cuma, 18:29
 */

#include "Node.h"
#include <iostream>
#include <cmath>
//#include <list>

// Constructors & Destructors
Node::Node() {
}

Node::Node(const Node& orig) {
    id = orig.getId();
    Gt = orig.getGt();
    Gr = orig.getGr();
    x = orig.getX();
    y = orig.getY();
}

Node::~Node() {
}

Node::Node(unsigned idIn, double xIn, double yIn) : x(xIn), y(yIn), id(idIn){
    Gt = 10;
    Gr = 10;
    
    previous == NULL;
}

// Member Functions
void Node::print() const {
    //std::cout << "Id: " << id << "\tX: " << x << "\t" << "Y: " << y << std::endl;
    //std::cout << "Cost: " << cost << "\tVisited: " << visited << std::endl;
    std::cout << x << "\t" << y << std::endl;
}

double Node::dist(const Node& node) const{
    return sqrt(pow(x-node.getX(),2) + pow(y-node.getY(),2));
}

void Node::addAdj(Node* n){
    adjacentList.push_back(n);
}

void Node::printAdj() const {
    std::cout << "Adjacent list of node with id: " << id << std::endl;
    for(std::list<Node*>::const_iterator it = adjacentList.begin();
            it != adjacentList.end(); ++it){
        std::cout << (*it)->getId() << "\t";
    }
    std::cout << std::endl << std::endl;
}