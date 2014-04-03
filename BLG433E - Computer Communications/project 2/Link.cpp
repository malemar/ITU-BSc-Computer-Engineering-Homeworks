/* 
 * File:   Link.cpp
 * Author: Volkan
 * 
 */

#include "Link.h"
#include <cmath>
#include <iostream>

#define LAMBDA 0.125    // wave length (m)
#define PMAX 0.0032     // Max power (W)
#define PTH 0.0000001   // Threshold power (W)
#define PI 3.1419

// Constructors & Destructors
Link::Link() {
}

Link::Link(const Link& orig) {
}

Link::~Link() {
}

Link::Link(const Node* n1, const Node* n2) : node1(n1), node2(n2){
    double d = n1->dist(*n2);   // distance between 2 nodes
    //std::cout << n1->getId() << " -- " << n2->getId() << " : " << d << std::endl;
    
    // Pr = Pt * Gt * Gr * LAMBDA^2 / (4*PI*d)^2        || Original Formula
    // Pt = Pr * (4*PI*d)^2 / (LAMBDA^2 * Gt * Gr)      || Pt 
    // Pr = PTH
    
    double Pt = PTH * pow((4*PI*d),2) / (pow(LAMBDA,2)*n1->getGt()*n2->getGr());
    if(Pt > PMAX)
        cost = 999;
    else
        cost = Pt;
}

// Member Functions
bool Link::contains(const Node* node) const {
    if(node == node1 || node == node2) 
        return true;
    else        
        return false;
}

void Link::print() const {
    std::cout << node1->getId() << " ---- " 
            << node2->getId() << ": " << cost << std::endl;
}
