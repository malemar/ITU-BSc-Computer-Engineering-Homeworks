/* 
 * File:   Graph.cpp
 * Author: varaquilex
 * 
 * Created on March 23, 2013, 12:56 AM
 */

#include "Graph.h"
#include <iostream>
#include <stack>

using namespace std;

Graph::Graph() {
}

Graph::Graph(const Graph& orig) {
}

Graph::~Graph() {
}

//getters & setters
list<Node*> Graph::getNodeList() { return this->nodeList;}
void Graph::setMaxDistance(const int i) { this->maxDistance = i; }
int Graph::getMaxDistance() const { return maxDistance; }
list<Node*>* Graph::getNodeListAddr() { return &(this->nodeList); }


//member functions
void Graph::addNode(Node* n, Node* adj){
    if(isInList(n) == false){    //if node is not in the list   0
        this->nodeList.push_back(n);   //add it to the list
        
        //if its adjacent node is NOT in the list, add it too
        bool found = false;
        list<Node*>::const_iterator it;
        for(it = nodeList.begin() ; it != nodeList.end() ; it++){
            if((*it)->getId() == adj->getId()){
                found = true;
                break;
            }
        }
        if(found == false){  //if its adjacent node is NOT in the list, add it too  00
            this->nodeList.push_back(adj);
            n->addAdjacent(adj);
        }   //01
        else   //if the adjacent node is in the list already, add it to the adjList of the Node n
            n->addAdjacent(*it);
    }
    else{   //if the node IS in the list
        bool found = false; //find adj. node in the list
        list<Node*>::const_iterator it;
        for(it = nodeList.begin() ; it != nodeList.end() ; it++){
            if((*it)->getId() == adj->getId()){
                found = true;
                break;
            }
        }
        if(found == true){  //if the adjacent node is in the list already  //11
            for(list<Node*>::const_iterator ite = nodeList.begin() ; ite != nodeList.end() ; ite++){
                if((*ite)->getId() == n->getId()){  //add it(adj node) to the node n's adjacent node list
                    (*ite)->addAdjacent(*it);
                    break;
                }
            }
        }
        else{   //if the node is in the list but not the adj node, add it to the list
            this->nodeList.push_back(adj);
            for(list<Node*>::const_iterator ite = nodeList.begin() ; ite != nodeList.end() ; ite++){
                if((*ite)->getId() == n->getId()){
                    (*ite)->addAdjacent(adj);
                    break;
                }
            }    //10
        }
    }
    return;
}
void Graph::printNodeList() const {
    cout << "Graph with max Distance: " << this->maxDistance << " and element count: " << this->nodeList.size() << endl;
    for(list<Node*>::const_iterator it = nodeList.begin() ; it != nodeList.end() ; it++){
        cout << "Node #" << (*it)->getId() << ": (Distance = " << (*it)->getDistance() << ")" << endl << "  Adjacents: ";
        (*it)->printAdjList();
        cout << endl;
    }
    return;
}
bool Graph::isInList(Node* n) const {
    for(list<Node*>::const_iterator it = nodeList.begin() ; it != nodeList.end() ; it++){
        if(n->getId() == (*it)->getId())
            return true;
    }
    return false;
}
void Graph::dijkstra(){
    this->maxDistance = 0;
    list<Node*> set = this->getNodeList();  //a copy is returned
                                            //to determine the distances
    Node* n = set.front();  //root node in the first iteration
    while(set.empty() == false){
        for(list<Node*>::iterator it = set.begin() ; it != set.end() ; it++){
            if(n->getDistance() > (*it)->getDistance()) //get the smallest distance node
                n = (*it);
        }
        
        set.remove(n);  //remove n from the list
        
		if(n->getDistance()==1000){ //if distance is infinity (if last element is visited)
			n->setDistance(n->getPrevious()->getDistance()+1);  //decrease key
			if(n->getPrevious()->getDistance()+1 > maxDistance) //set the max distance
				maxDistance = n->getPrevious()->getDistance()+1;
			break;  //terminate
		}
        
        int distance;   //distance between n and its adjacent nodes
		for(list<Node*>::iterator it = n->getAdjList()->begin() ; it != n->getAdjList()->end() ; it++){
			distance = n->getDistance()+1;
			if((*it)->getDistance()==0){    //if the adjacent node is the root
					n->setDistance(1);  //set distance to 1
			}   //if its not the root
			else if(distance < (*it)->getDistance()){   //and distance is less than its current value
				(*it)->setDistance(distance);   //decrease key (distance)
				(*it)->setPrevious(n);  //make the adjacent node's previous node n
				if(distance > maxDistance)  //set max distance
					maxDistance = distance;
			}
            
		}
        n = set.front();    //the list is iterated
    }
}
