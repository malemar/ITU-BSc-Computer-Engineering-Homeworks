/* 
 * File:   Graph.h
 * Author: varaquilex
 *
 * Created on March 23, 2013, 12:56 AM
 */

#ifndef GRAPH_H
#define	GRAPH_H

#include "Node.h"
#include <list>

using namespace std;

class Graph {
public:
    //constructors & destructors
    Graph();
    Graph(const Graph& orig);
    virtual ~Graph();
    
    //getters & setters
    list<Node*> getNodeList();
    list<Node*>* getNodeListAddr();
    void setMaxDistance(const int);
    int getMaxDistance() const;
    
    //member functions
    void addNode(Node*, Node*);
    void printNodeList() const;
    bool isInList(Node*) const;
    void dijkstra();    //sets the distances of nodes in the graph
private:
    list<Node*> nodeList;
    int maxDistance;    // = max level
};

#endif	/* GRAPH_H */

