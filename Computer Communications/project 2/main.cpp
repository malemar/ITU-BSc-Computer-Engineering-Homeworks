/************************************************/
/*	BLG435E - Computer Communications	*/
/*		Fall 2013			*/
/*						*/
/*              Volkan İlbeyli			*/
/*		   040100118                    */
/*						*/
/*	    Instructor: Sema Oktuğ              */
/************************************************/

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <string>

#include <list>
#include <queue>
#include <stack>

#include "Node.h"
#include "Link.h"
#include "Route.h"

using namespace std;

#define PB push_back
#define whole(v) v.begin(), v.end()
#define FOR(i,a,b) for(unsigned i=a ; i<b ; i++)
#define FORE(i,n) FOR(i,0,n)

typedef list<Node*> lN;
typedef list<Link*> lL;
typedef list<Route*> lR;
typedef list<Node*>::iterator lNi;
typedef list<Node*>::const_iterator lNci;
typedef list<Link*>::iterator lLi;
typedef list<Link*>::const_iterator lLci;
typedef list<Route*>::iterator lRi;
typedef list<Route*>::const_iterator lRci;

template<class Container>
void print(const Container& c) {
#ifdef Cpp11
    for (auto const &item : c)
        cout << item << endl;
#else
    copy(whole(c), ostream_iterator<typename Container::value_type > (cout, "\n"));
#endif
}

// function prototypes
bool readFile(const char*, lN&);
void randInput();
void constituteTopology(const lN&, lL&);
void routeEmUp(const lN&, const lL&, lR&);
void dijkstra(Node*, Node*, const lN&, const lL&, lR&);
Node* smallest(const lN&);
double getCostFromLinks(const Node*, const Node*, const lL&);

int main(int argc, char** argv) {
    
    srand((unsigned)time(NULL));
    //randInput();
    
    lN nodes;   // Node* list
    lL links;   // Link* list
    lR routes;  // Route* list
    
    // parameter check
    if(argc != 2){
        cout << "Invalid command line parameters." << endl;
        cout << "Example: ./a.out input.txt" << endl;
        return -1;
    }
    
    // read file, get the nodes from file
    if(readFile(argv[1], nodes) == false){
        return -1;
    }
    
    constituteTopology(nodes, links);   // calculate link costs
    routeEmUp(nodes, links, routes);
    
    return 0;
}

bool readFile(const char* fileName, lN& nodes){
    FILE* fp = fopen(fileName,"r");
    if(fp == NULL){
        fclose(fp);
        cout << "Error opening file: " << fileName << endl;
        return false;
    }
    
    FORE(i,10){
        double x, y;
        fscanf(fp, "%lf\t%lf", &x, &y);
        nodes.PB(new Node(i,x,y));
        //nodes.back()->print();        
    }
    
    fclose(fp);
    return true;
}

void randInput(){       // create random input
    FILE* fp = fopen("input.txt","w");
    
    FORE(i, 10){
        fprintf(fp, "%lf\t%lf\n", ((double)(rand()%5000))/100,
                ((double)(rand()%5000))/100);
    }
    
    fclose(fp);
    return;
}

void constituteTopology(const lN& nodes, lL& links){
        
    // for all nodes
    for(lNci it = nodes.begin() ; it != nodes.end() ; ++it){     
        // check remaining nodes to create links
        lNci ite = it;  // make a copy of the current node
        ++ite;          // point to next node
        for(ite; ite != nodes.end() ; ++ite){   // iterate to the end     
            links.PB(new Link(*it, *ite));      // create a link between nodes
            //links.back()->print();
            
            if(links.back()->getCost() < 999){  // if cost is not infinity
                (*it)->addAdj(*ite);            // add each node to others
                (*ite)->addAdj(*it);            // adjacent node list
            }
        }
        //cout << "Adj list of " << (*it)->getId() << endl;
        //(*it)->printAdj();
        //cout << endl;
    }
    
    return ;
}

void routeEmUp(const lN& nodes, const lL& links, lR& routes){
    
    // for all nodes
    for(lNci it = nodes.begin() ; it != nodes.end() ; ++it){  
        
        // check remaining nodes to create links
        lNci ite = it;  // make a copy of the current node
        ++ite;          // point to next node
        for(ite; ite != nodes.end() ; ++ite){   // iterate to the end
            // initialize a route from source to destination
            routes.PB(new Route((*it), (*ite)));
            
            dijkstra(*it, *ite, nodes, links, routes); 
            //getchar();
            cout << "-------------------------------" << endl;
        }
    }
    
    // print routing table
    cout << "============================================================" << endl;
    cout << "====================   Routing Table   =====================" << endl;
    cout << "============================================================" << endl;
    cout << fixed;
    cout << setprecision(8);
    for(lRi it = routes.begin() ; it != routes.end() ; ++it){
        if((*it)->getPathCost() != 999){
            cout << (*it)->getN1()->getId() << " -> " << (*it)->getN2()->getId() 
                    << " costs " << (*it)->getPathCost() << " == ";
            (*it)->printPath();
            cout << endl;
        }
    }
    
    cout << "===========================================================" << endl;
    return;
}

void dijkstra(Node* n1, Node* n2, const lN& nodes, const lL& links, lR& routes){
/*      source: http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
 
 1  function Dijkstra(Graph, source):
 2      for each vertex v in Graph:                           // Initializations
 3          dist[v]      := infinity;                         // Mark distances from source to v as not yet computed
 4          visited[v]   := false;                            // Mark all nodes as unvisited
 5          previous[v]  := undefined;                        // Previous node in optimal path from source
 6      end for
 7      
 8      dist[source]  := 0;                                   // Distance from source to itself is zero
 9      insert source into Q;                                 // Start off with the source node
10                                                                
11      while Q is not empty:                                 // The main loop
12          u := vertex in Q with smallest distance in dist[] and has not been visited;  // Source node in first case
13          remove u from Q;
14          visited[u] := true                                // mark this node as visited
15          
16          for each neighbor v of u:   
17              alt := dist[u] + dist_between(u, v);          // accumulate shortest dist from source
18              if alt < dist[v]:                                 
19                  dist[v]  := alt;                          // keep the shortest dist from src to v
20                  previous[v]  := u;
21                  if !visited[v]:
22                       insert v into Q;                          // Add unvisited v into the Q to be processed
23                  end if
24              end if
25          end for
26      end while
27      return dist;
28  endfunction

 If we are only interested in a shortest path between vertices source and target, 
 we can terminate the search at line 12 if u = target. 
 Now we can read the shortest path from source to target by reverse iteration:

1  S := empty sequence
2  u := target
3  while previous[u] is defined:                              // Construct the shortest path with a stack S
4      insert u at the beginning of S                         // Push the vertex into the stack
5      u := previous[u]                                       // Traverse from target to source
6  end while ;
 
*/

    cout << "Dijkstra called for nodes " << n1->getId() << " and "
            << n2->getId() << endl << "----------------------" << endl;;
    
    double totalCost = 0;
    lN Q;       // Q above
    for(lNci it = nodes.begin(); it != nodes.end() ; ++it){
        (*it)->setCost(999);
        (*it)->setVisited(false);
        (*it)->setPrev(NULL);
    }
    
    n1->setCost(0);    
    Q.PB(n1);
    
    Node* u = NULL;
    while(Q.empty() == false){
        if(u == n2)     // if target node reached
            break;      // terminate the algorithm
        
        
        u = smallest(Q);    // smallest distance & un-visited
        Q.remove(u);
        u->setVisited(true);
                
        for(lNci it = u->getAdjL().begin() ; it != u->getAdjL().end() ; ++it){
            if((*it)->getVisited() == false){
                totalCost = u->getCost() + getCostFromLinks(u, *it, links);
                if(totalCost < (*it)->getCost()){
                    cout << "totalCost: " << totalCost << " < node " 
                           << (*it)->getId() << "'s cost: " << (*it)->getCost() << endl;
                    (*it)->setCost(totalCost);
                    (*it)->setPrev(u);
                    cout << "Node " << (*it)->getId() << "'s new cost: " 
                            << (*it)->getCost() << ", Previous Node: Node "
                            << (*it)->getPrev()->getId() << endl << endl;
                    if((*it)->getVisited() == false)
                        Q.PB(*it);
                }
            } // end if visited
        } // end for (adj list)
    } // end while
    
    if(n2->getVisited() == false){
        totalCost = 999;
    }
    
    
    cout << "Total cost between nodes " << n1->getId() << " and "
            << n2->getId() << " = " << n2->getCost() << endl;
    
    if(n2->getCost() != 999){
        
        // Save the path by going back from target to source
        stack<Node*> S;    
        u = n2;     // start from target
        while(u != NULL){
            S.push(u);
            u = u->getPrev();
        }

        // print the path from source to target
        while(S.empty() == false){
            cout << S.top()->getId();
            routes.back()->addNode(S.top());
            S.pop();
            if(S.empty() == false) cout <<  " --> ";
        }
        
        
    }
    else{
        cout << "THERE IS NO PATH FROM " << n1->getId() 
                << " TO " << n2->getId() << endl;
    }
    routes.back()->setPathCost(n2->getCost());
    
    cout << endl;
    return;
}

Node* smallest(const lN& nodes){ // returns unvisited node with smallest cost
    double tiniest = 1000;
    Node* n = NULL;
    for(lNci it = nodes.begin() ; it != nodes.end() ; ++it){
        if(tiniest > (*it)->getCost() && (*it)->getVisited() == false){
            tiniest = (*it)->getCost();
            n = (*it);
        }
    }
    
    return n;
}

double getCostFromLinks(const Node* n1, const Node* n2, const lL& links){
    for(lLci it = links.begin() ; it != links.end() ; ++it){
        
        // 2 if's are practically the same, the reason for 2 separate if's
        // is to print n1 and n2 in the arriving order
        if( (   (*it)->getNode1()->getId() == n1->getId() && 
                (*it)->getNode2()->getId() == n2->getId() )){
            
            cout << (*it)->getNode1()->getId() << " " 
                    << (*it)->getNode2()->getId() << endl;
            return (*it)->getCost();
        }
        
        if ((   (*it)->getNode1()->getId() == n2->getId() && 
                (*it)->getNode2()->getId() == n1->getId() )){
            cout << (*it)->getNode2()->getId() << " " 
                    << (*it)->getNode1()->getId() << endl;
            return (*it)->getCost();
        }
    }
}