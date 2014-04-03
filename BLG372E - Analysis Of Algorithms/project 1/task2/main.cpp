/****************************************/
/*	BLG372E - Analysis of Algorithms	*/
/*				Spring 2013				*/
/*										*/
/*			Volkan İlbeyli				*/
/*			   040100118                */
/*										*/
/*		Instructor: Hazım Ekenel        */
/****************************************/

#include <iostream>
#include <cstdio>
#include "Graph.h"
#include "Node.h"

using namespace std;

bool writeToFile(Graph[5]);
bool readFromFile(const char*, Graph[5]);
int checkConnection(list<Node*>);   //check connections in the same level

int main(int argc, char* argv[]) {
    if(argc != 2){  //error check
        cout << "Invalid arguments, example: ./a.out data2.txt" << endl;
        cout << "Returning from main() . . ." << endl;
        return -1;
    }
    
    Graph graph[5];
    if(readFromFile(argv[1], graph) == false){    //read and error check
        cout << "Returning from main(). . ." << endl;
        return -1;
    }
    
    //print graphs' contents
    for(int i=0 ; i<5 ; i++){
        cout << "graph[" << i << "] node list:" << endl;
        graph[i].printNodeList();
        cout << endl << endl;
    }
    
    if(writeToFile(graph) == false){    //write and error check
        cout << "Returning from main(). . ." << endl;
        return -1;
    }
    
    return 0;
}

bool readFromFile(const char* fileName, Graph graph[5]){
    //reads from file and initializes the graphs by
    //setting the distances of each node from the root
    FILE* fp;
    
    fp = fopen(fileName, "r");
    if(fp == NULL){ //error check
        cout << "Error reading file: " << fileName << endl;
        return false;
    }
    
    //read content
    int id, adjId;
    for(int j=0 ; j<5 ; j++){   //graph counter
        int setLength;  // # of connections given in the file
        fscanf(fp, "%d\n", &setLength);
        for(int i=0 ; i<setLength ; i++){
            fscanf(fp, "%d %d\n", &id, &adjId); 
            graph[j].addNode(new Node(id), new Node(adjId));
        }
        graph[j].dijkstra();    //sets the nodes' distances to root
    }
    fclose(fp);
    return true;
}
bool writeToFile(Graph graph[5]){
    FILE* fp;
    fp = fopen("output2.txt","w");
    if(fp == NULL){ //error check
        cout << "Error opening file: output2.txt" << endl;
        return false;
    }
    
//count pairs
int pairCount[5] = {0};
list<Node*> nodeSet;    //nodes in a graph with same distance
    
for(int j=0 ; j<5 ; j++){   //do for all 5 graphs
    for(int i=1 ; i<=graph[j].getMaxDistance() ; i++){  //start from 2nd level (not root), iterate to the max level
        //add the nodes that are in the same level to the nodeSet
        for(list<Node*>::iterator it = graph[j].getNodeListAddr()->begin() ; it != graph[j].getNodeListAddr()->end() ; it++){
            if((*it)->getDistance() == i)   //if in the same level
                nodeSet.push_back(*it); //add to nodeSet
        }
            
        if(nodeSet.size() <= 1){    //if there are less than 2 nodes in the set
            nodeSet.clear();    //clear the list
            break;          //and move to next level
        }
            
        //if there are more than 1 nodes in the same level
        //check if they have a connection with each other
        pairCount[j] += checkConnection(nodeSet);   //and count meanwhile
        nodeSet.clear();    //clear the set after checking the connections
    }
    cout << "*** graph " << j << ": "  << pairCount[j] << " ***" << endl;
}
    
    //write the lines to file
    for(int i=0 ; i < 5 ; i++){
        if(i != 4)
            fprintf(fp,"%d\n",pairCount[i]);
        else
            fprintf(fp,"%d",pairCount[i]);
    }
    
    fclose(fp);
    return true;
}

int checkConnection(list<Node*> nodeSet){
    //this function counts the connected pairs in the same level
    //(nodeSet is given such that the nodes it contain are in the same level)
    int count = 0;
    for(list<Node*>::iterator it = nodeSet.begin() ; it != nodeSet.end() ; it++){
        if(++it != nodeSet.end()){  //compare an element with others by starting from the next element
            list<Node*>::iterator ite = it;	//2nd loops loop variable is set to the next element
            it--;	//iterator should be set back to its original position
            for( ite ; ite != nodeSet.end() ; ite++){   //checking adj list of node
                if((*it)->isInAdjList(*ite))
                    count++;
            }
        }
        else 
            break;
    }
    return count;
}