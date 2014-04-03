/* 
 * File:   Node.h
 * Author: Volkan
 *
 * Created on 29 Kasım 2013 Cuma, 18:29
 */

#ifndef NODE_H
#define	NODE_H

#include <list>

class Node {
public:
    
    // Constructors & Destructors
    Node();
    Node(const Node& orig);
    Node(unsigned, double, double);
    virtual ~Node();
    
    // Getters & Setters
    inline double getX() const { return x; }
    inline double getY() const { return y; }
    inline double getGt() const { return Gt; }
    inline double getGr() const { return Gr; }
    inline unsigned getId() const { return id; }
    inline bool getVisited() const { return visited; }
    inline double getCost() const { return cost; }
    inline void setId(unsigned i) { id = i; }
    inline void setVisited(bool b) { visited = b; }
    inline void setCost(double c) { this->cost = c; }
    inline const std::list<Node*>& getAdjL() const { return adjacentList; }
    inline Node* getPrev() const { return previous; }
    inline void setPrev(Node* n) { previous = n; }
    
    // Member Functions
    void print() const;
    double dist(const Node&) const;
    void addAdj(Node*);
    void printAdj() const;
    
private:
    unsigned id;
    double x,y;         // coordinates
    double Gt, Gr;      // transmit/receive antenna gains
    std::list<Node*> adjacentList;
    
    // dijkstra properties
    bool visited;
    double cost;
    Node* previous;
};

#endif	/* NODE_H */

