/* 
 * File:   Edge.cpp
 * Author: Volkan İlbeyli
 * 
 * Created on April 10, 2013, 11:06 AM
 */

#include "Edge.h"
#include <string>
#include <iostream>
using namespace std;

//constructors & destructors
Edge::Edge() {
}
Edge::Edge(const Edge& orig) {
}
Edge::~Edge() {
}
Edge::Edge(unsigned const int cost, const int c1, const int c2){
    this->cost = cost;
    this->city1 = c1;
    this->city2 = c2;
}
bool Edge::operator<(const Edge* e) const {
    return this->cost < e->GetCost();
}
