/* 
 * File:   Node.h
 * Author: varaquilex
 *
 * Created on March 22, 2013, 11:39 PM
 */

#ifndef NODE_H
#define	NODE_H

#include <list>

using namespace std;

class Node {
public:
    //constructors & destructors
    Node();
    Node(const Node& orig);
    Node(const int);
    virtual ~Node();
    
    //getters & setters
    int getDistance() const;
    void setDistance(const int);
    int getId() const;
    list<Node*>* getAdjList();
    void setVisited(bool);
    bool getVisited() const;
    void setPrevious(Node*);
    Node* getPrevious() const;
    
    //member functions
    void addAdjacent(Node*);
    void printAdjList() const;
    bool isInAdjList(Node*) const;
private:
    int id;
    int distance;   //distance from the root
    list<Node*> adjacentList;
    bool visited;   //for dijkstra's algorithm
    Node* previous; //the node which has this node as its adjacent
};

#endif	/* NODE_H */

