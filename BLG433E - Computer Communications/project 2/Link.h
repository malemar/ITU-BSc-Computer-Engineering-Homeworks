/* 
 * File:   Link.h
 * Author: Volkan
 *
 * Created on 01 Aralık 2013 Pazar, 18:21
 */

#ifndef LINK_H
#define	LINK_H

#include "Node.h"

class Link {
public:
    
    // constructors & destructors
    Link();
    Link(const Link& orig);
    virtual ~Link();
    Link(const Node*, const Node*);
    
    // getters & setters
    inline double getCost() const { return cost; }
    inline const Node* getNode1() const { return node1; }
    inline const Node* getNode2() const { return node2; }
    
    
    // member functions
    bool contains(const Node*) const;    // contains the node?
    void print() const;
private:
    const Node* node1, *node2;
    double cost;
};

#endif	/* LINK_H */

