/* 
 * File:   Node.cpp
 * Author: varaquilex
 * 
 * Created on March 22, 2013, 11:39 PM
 */

#include "Node.h"
#include <iostream>

using namespace std;

//constructors & destructors
Node::Node() {
}

Node::Node(const Node& orig) {
    this->distance = orig.distance;
    this->id = orig.id;
    this->adjacentList = orig.adjacentList;
}

Node::~Node() {
}

Node::Node(const int id){
    this->id = id;
    if(id == 1){    //root node
      setDistance(0);
    }
    else{
        setDistance(1000);
    }
    visited = false;
}

//getters & setters
int Node::getDistance() const { return distance; }
void Node::setDistance(const int distance) { this->distance = distance; }
int Node::getId() const { return id; }
list<Node*>* Node::getAdjList(){ return &(this->adjacentList); }
void Node::setVisited(bool s){ visited = s ;}
bool Node::getVisited() const { return visited; }
void Node::setPrevious(Node* n) { previous = n; }
Node* Node::getPrevious() const { return previous; }

//member functions
void Node::addAdjacent(Node* n) {
    this->adjacentList.push_back(n);
}
void Node::printAdjList() const {
    for(list<Node*>::const_iterator it = adjacentList.begin() ; it != adjacentList.end() ; it++){
        cout << (*it)->getId() << " ";
    }
    cout << endl;
    return;
}
bool Node::isInAdjList(Node* n) const {
    for(list<Node*>::const_iterator it = adjacentList.begin() ; it != adjacentList.end() ; it++){
        if(n->getId() == (*it)->getId())
            return true;
    }
    return false;
}