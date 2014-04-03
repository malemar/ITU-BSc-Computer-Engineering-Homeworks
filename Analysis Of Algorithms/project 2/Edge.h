/* 
 * File:   Edge.h
 * Author: Volkan İlbeyli
 *
 * Created on April 10, 2013, 11:06 AM
 */

#ifndef EDGE_H
#define	EDGE_H

using namespace std;

class Edge {
public:
    //constructors & destructors
    Edge();
    Edge(const Edge& orig);
    virtual ~Edge();
    Edge(unsigned const int, const int, const int);
    
    //getters & setters
    unsigned int GetCost() const {return cost;}
    unsigned int GetCity1() const {return city1;}
    unsigned int GetCity2() const {return city2;}

    //member functions
    bool operator<(const Edge*) const;
private:
    unsigned int cost;  //cost of the edge
    unsigned int city1, city2;   //cities that the edge connect
};

#endif	/* EDGE_H */

