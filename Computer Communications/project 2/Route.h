/* 
 * File:   Route.h
 * Author: Volkan
 *
 * Created on 02 Aralık 2013 Pazartesi, 02:42
 */

#ifndef ROUTE_H
#define	ROUTE_H

#include "Node.h"

class Route {
public:

    // constructors & destructors
    Route();
    Route(const Route& orig);
    virtual ~Route();
    Route(const Node*, const Node*);

    // getters & setters
    void setPathCost(double c) { pathCost = c; }
    double getPathCost() const { return pathCost; }
    const Node* getN1() const  { return node1; }
    const Node* getN2() const  { return node2; }
    
    // member functions
    void addNode(const Node*);
    void printPath() const;

private:
    const Node* node1, *node2;    // terminal nodes
    std::list<const Node*> path;  // all nodes along the path
    double pathCost;
};

#endif	/* ROUTE_H */

