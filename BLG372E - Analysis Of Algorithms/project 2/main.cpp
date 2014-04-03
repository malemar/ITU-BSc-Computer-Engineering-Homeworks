/*
*	Nick: Varaquilex
*	Name: Volkan İlbeyli
*	Mail: volkan@ilbeyli.info
*/

/****************************************/
/*	BLG372E - Analysis of Algorithms	*/
/*				Spring 2013				*/
/*				Project II				*/
/*                                      */
/*			Volkan İlbeyli				*/
/*			   040100118                */
/*										*/
/*		Instructor: Hazım Ekenel        */
/****************************************/

#include <iostream> //cout, endl 
#include <cstdio>   //fopen(), fclose(), fscanf(), fseek(), SEEK_SET
#include <cstring>  //strtok(), strcpy()
#include <ctime>    //time()
#include "Edge.h"
#include <list>
#include <set>
#include <vector>
#include <map>
#include <cstdlib>  //srand(), rand(), atoi()

using namespace std;

//comparator functor for Edge* list
struct CompareCost{ //since two pointers cannot be compared in a member function
   bool operator () (const Edge* e1, const Edge* e2){
      return e1->operator <(e2);	//see how member operator< is invoked
   }   
};

typedef vector<Edge*> vE;
typedef vector<Edge*>::iterator vEit;
typedef list<Edge*> lE;
typedef map<string, int> msi;
typedef list< set<int> > lsi;
typedef set<int> si;

#define Iterate(it,c) for(__typeof(c.begin()) it = c.begin() ; it != c.end() ; it++)
#define whole(v) v.begin(), v.end()

//function prototypes
bool read_input(string, string, msi&, vE&);
void print_vector(vE&, msi&);
lE randEdge(vE, unsigned const int);
int print_list(lE&, msi&);
int print_list(lE&);
string find_key(msi&, int);
void print_map(msi&);
void print_sets(lsi&);
void print_set(si&);
lE kruskal(lE&, msi&);
lE kruskal(lE&, msi&, bool);
double kruskal_time(lE&, msi&);

int main(int argc, char** argv) {
    vE edges;       //vector<Edge*>
    lE edge_list;   //list<Edge*>
    msi cities;     //map<int, string>
    
    if(argc != 4){  //program input error check
        cout << "Invalid argument count." << endl;
        cout << "Example: ./a.out Cities_10.txt Flights_10.txt result10.txt" << endl;
        cout << "Returning from main()..." << endl;
        return -1;
    }
    srand((unsigned)time(NULL));    //seed random
    if(read_input(argv[1], argv[2], cities, edges) == false){
        cout << "Returning from main(). . ." << endl;
        return -1;
    }
    FILE* out = fopen(argv[3],"w");
    if(out == NULL){
        cout << "Error opening file: " << argv[3] << endl;
        cout << "Returning from main()..." << endl;
        return -1;
    }
    
    cout << endl << endl << "Keep pressing enter to proceed"
            " during the running time" << endl << endl << endl;
    getchar();
    
    //print out initial data
    cout << "Initial graph and city map" << endl << endl;
    print_map(cities);
    getchar();
    print_vector(edges, cities);
    getchar();
    
    //get randomly picked edge list and print information
    cout << endl;
    cout << "WARNING! Given large inputs, choosing a number close to" 
            << " city count might result in very long loops for "
            << "finding an MST since randomly chosen edges might not "
            << "build a connected graph." << endl << endl;
    cout << "Suggested minimum edge number: 1.5*city-count" << endl << endl;
    cout << "Enter the number of edges that will be randomly chosen: ";
    unsigned int K;
    cin >> K;   //number of edges that will be randomly chosen
    lE MST;     //minimum spanning tree
    
    /**********************************************/
    /*****************  TEST 1  *******************/
    /**********************************************/
    cout << endl << endl << "TEST 1 STARTED. . ." << endl;
    fprintf(out,"TEST 1\n");
    char choice;
    time_t t;
    cout << "Do you want to see the steps of the algorithm(Y/N)?: ";
    cin >> choice;      //printing steps is just for test 1
    do{ //test 1
        t = time(NULL);
        edge_list = randEdge(edges, K);
        if(edge_list.size() < cities.size()-1){
            cout << "Too few edges to form a Tree" << endl;
            cout << "Enter the number of edges that will be randomly chosen: ";
            cin >> K;
            continue;
        }
        cout << "Edge vector(contains all edges) size: " << edges.size() << endl;
        cout << "Edge list(to be Kruskal'd) size: " << edge_list.size() << endl << endl;
        switch(choice){
            case 'y':
            case 'Y':
                MST = kruskal(edge_list, cities, true);
                break;
            case 'n':
            case 'N':
                MST = kruskal(edge_list, cities);
                break;
            default:
                cout << "Wrong input, Running kruskal() " <<
                        "without printing steps." << endl;
                MST = kruskal(edge_list, cities);
                break;
        }
    }while(MST.size() != cities.size()-1);
    getchar(); getchar();
    //MST found, print MST info
    cout << "Minimum Spanning Tree: " << endl;
    int totalCost = print_list(MST);
    cout << "With a total cost of " << totalCost << endl;
    //print_list(MST, cities);  //for printing the city names
    double runtime;
    runtime = kruskal_time(edge_list,cities); //compute time
    
    //write to file
    Iterate(it, MST){
        fprintf(out,"%d %d %d\n", 
                (*it)->GetCity1(), 
                (*it)->GetCity2(),
                (*it)->GetCost()); 
    }
    fprintf(out, "Total cost: %d\n", totalCost);
    fprintf(out, "Running time: %.2lfms\n", runtime);
    
    cout << endl << "TEST 1 ENDED. . ." << endl;
    edge_list.clear();
    getchar();
    
    /**********************************************/
    /*****************  TEST 2  *******************/
    /**********************************************/
    cout << endl << "TEST 2 STARTED. . ." << endl;
    fprintf(out,"\nTEST 2\n");
    do{ //test 2
        t = time(NULL);
        edge_list = randEdge(edges, 2*K);
        //print_list(edge_list, cities);
        cout << "Edge vector(contains all edges) size: " << edges.size() << endl;
        cout << "Edge list(to be Kruskal'd) size: " << edge_list.size() << endl << endl;
        
        MST = kruskal(edge_list, cities);
        getchar();
    }while(MST.size() != cities.size()-1);
    
    //MST found, print MST info
    cout << "Minimum Spanning Tree: " << endl;
    totalCost = print_list(MST);
    cout << "With a total cost of " << totalCost << endl;
    
    runtime = kruskal_time(edge_list,cities);
    
    //dump to file
    Iterate(it, MST){
        fprintf(out,"%d %d %d\n", 
                (*it)->GetCity1(), 
                (*it)->GetCity2(),
                (*it)->GetCost()); 
    }
    fprintf(out, "Total cost: %d\n", totalCost);
    fprintf(out, "Running time: %.2lfms\n", runtime);
    
    cout << endl << "TEST 2 ENDED. . ." << endl;
    edge_list.clear();
    getchar();
    
    /**********************************************/
    /*****************  TEST 3  *******************/
    /**********************************************/
    cout << endl << "TEST 3 STARTED. . ." << endl;
    fprintf(out,"\nTEST 3\n");
    do{ //test 3
        t = time(NULL);
        edge_list = randEdge(edges, 3*K);
        //print_list(edge_list, cities);
        cout << "Edge vector(contains all edges) size: " << edges.size() << endl;
        cout << "Edge list(to be Kruskal'd) size: " << edge_list.size() << endl << endl;
        
        MST = kruskal(edge_list, cities);
        getchar();
    }while(MST.size() != cities.size()-1);
    
    //MST found, print MST info
    cout << "Minimum Spanning Tree: " << endl;
    totalCost = print_list(MST, cities);
    cout << "With a total cost of " << totalCost << endl;
    
    runtime = kruskal_time(edge_list,cities);
    
    //dump to file
    Iterate(it, MST){
        fprintf(out,"%d %d %d\n", 
                (*it)->GetCity1(), 
                (*it)->GetCity2(),
                (*it)->GetCost()); 
    }
    fprintf(out, "Total cost: %d\n", totalCost);
    fprintf(out, "Running time: %.2lfms\n", runtime);
    
    cout << endl << "TEST 3 ENDED. . ." << endl;
    
    fclose(out);
    return 0;
}

bool read_input(string inputC, string inputF, msi& cities,vE& edges){
    FILE* fp;
    unsigned int cost;
    char buffer[2][128];    //city names
    char line_buffer[256];
    char* token;
    
    /*********************************************/
    /***********  READ THE CITIES   **************/
    /*********************************************/
    fp = fopen(inputC.c_str(),"r");
    if(fp == NULL){ //open file & error check
        cout << "Error reading file: " << inputC <<  endl;
        return false;
    }
    
    int nLines=0;   //find the line count (city count)
    while(fgets(line_buffer, 255, fp))
        nLines++;
    fseek(fp, 0, SEEK_SET); //rewind file pointer
    
    //read the cities
    for(int i = 0 ; i < nLines ; i++){
        fscanf(fp, "%s\n", line_buffer);
        cities.insert(pair<string, int>(line_buffer, i));
    }
    fclose(fp);
    
    /*********************************************/
    /***********  READ THE FLIGHTS  **************/
    /*********************************************/
    fp = fopen(inputF.c_str(),"r");
    if(fp == NULL){ //open file & error check
        cout << "Error reading file: " << inputF << endl;
        return false;
    }
    
    //read the edges
    while(fscanf(fp, "%s\n", line_buffer) != EOF){
        //tokenize the line since the values are not separated by ' '
        int element_index = 0;  //for indexing city buffers, 0 & 1
        token = strtok(line_buffer,",");
        while(token){
            if(element_index < 2){  //tokenize cities
                strcpy(buffer[element_index], token); //city[index] <-- token
                token = strtok(NULL,",");
                element_index++;
            }
            else{
                cost = atoi(token); // cost <-- token
                token = strtok(NULL,",");
            }
        }   //tokenizing finished
        
        int c1 = cities.find(buffer[0])->second;
        int c2 = cities.find(buffer[1])->second;
        edges.push_back(new Edge(cost, c1, c2));
    }
    fclose(fp);
    return true;
}
string find_key(msi& cities, int value){
    Iterate(it,cities){
        if(it->second == value)
            return it->first;
    }
    return "NOT FOUND!";
}
void print_vector(vE& edges, msi& cities){
    cout << "Edge vector:" << endl; //if & else's are for pretty printing
    for(vEit it = edges.begin() ; it != edges.end() ; it++)
        if(strlen(find_key(cities, (*it)->GetCity1()).c_str()) 
                + strlen(find_key(cities, (*it)->GetCity2()).c_str()) <= 12
                && it-edges.begin() < 10){  
            cout << "edges[" << it-edges.begin() << "]: " 
                << find_key(cities, (*it)->GetCity1()) << " " 
                << find_key(cities, (*it)->GetCity2()) << "\t\t" 
                << (*it)->GetCost() << endl;
        }
        else if(strlen(find_key(cities, (*it)->GetCity1()).c_str()) 
                + strlen(find_key(cities, (*it)->GetCity2()).c_str()) < 12
                && it-edges.begin() >= 10){
            cout << "edges[" << it-edges.begin() << "]: " 
                << find_key(cities, (*it)->GetCity1()) << " " 
                << find_key(cities, (*it)->GetCity2()) << "\t\t" 
                << (*it)->GetCost() << endl;
        }
        else if(strlen(find_key(cities, (*it)->GetCity1()).c_str()) 
                + strlen(find_key(cities, (*it)->GetCity2()).c_str()) >= 12
                && it-edges.begin() >= 10){
            cout << "edges[" << it-edges.begin() << "]: " 
                << find_key(cities, (*it)->GetCity1()) << " " 
                << find_key(cities, (*it)->GetCity2()) << "\t" 
                << (*it)->GetCost() << endl;
        }
    cout << endl;
    return;
}
lE randEdge(vE edges, unsigned const int K){
    lE edge_list;
    cout << "Picking random edges..." << endl;
    
    if(K >= edges.size()){   //if K is too large
        cout << "Given K " << K << " and edges size " << edges.size() << endl;
        cout << "Number of randomly picked edges exceed the "
                << "edge count, all edges are included in the edge list"
                << endl;
        Iterate(it, edges)
            edge_list.push_back(*it);
        return edge_list;
    }
    
    //randomly pick K edges that will form the tree
    while(edge_list.size() < K){ 
        int index = rand()%edges.size();
        edge_list.push_back(edges[index]);
        edges.erase(edges.begin()+index);
    }
    
    return edge_list;
}
int print_list(lE& edge_list, msi& cities){    //prints edges with city names
    int cost = 0;
    cout << "Edge list (of size " << edge_list.size() << "):" << endl;
    Iterate(it, edge_list){ //if & else's are for pretty printing
        if(strlen(find_key(cities, (*it)->GetCity1()).c_str()) +
                strlen(find_key(cities, (*it)->GetCity2()).c_str()) <= 14){
            cout << find_key(cities, (*it)->GetCity1()) << " " 
                    << find_key(cities, (*it)->GetCity2()) << "\t\t" 
                    << (*it)->GetCost() << endl;
        }
        else{
            cout << find_key(cities, (*it)->GetCity1()) << " " 
                    << find_key(cities, (*it)->GetCity2()) << "\t" 
                    << (*it)->GetCost() << endl;
        }
        cost += (*it)->GetCost();
    }
    
    cout << endl;
    return cost;
}
int print_list(lE& edge_list){
    int cost;
    cout << "Edge list(without city names): " << endl;
    Iterate(it, edge_list){ //prints edges without city names
        cout << (*it)->GetCity1() << " "
                << (*it)->GetCity2() << " "
                << (*it)->GetCost() << endl;
        cost += (*it)->GetCost();
    }
    cout << endl;
    return cost;
}
void print_map(msi& cities){
    cout << "Cities map:" << endl;
    Iterate(it, cities){
        cout << it->first << " => " << it->second << endl;
    }
    cout << endl;
}
void print_sets(lsi& city_sets){
    cout << "Printing the sets (size " << city_sets.size() << "):" << endl;
    Iterate(it, city_sets){
        cout << "Set: ";
        Iterate(ite, (*it)){
            cout << *ite << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void print_set(si& set){
    cout << "Printing the (single) set:" << endl;
    Iterate(it, set){
        cout << *it << " ";
    }
    cout << endl;
}
lE kruskal(lE& edge_list, msi& cities){ //STEPS ARE NOT SHOWN
    unsigned int basic_step = 0;
    edge_list.sort(CompareCost());  //sort the edge list
    
    lE tree;    //tree, edge list
    lsi city_sets; //list of set of ints  
    si node;    //int set (node, single city)
    Iterate(it, cities){    //make the singleton city sets
        node.insert(it->second);
        city_sets.push_back(node);
        node.clear();
    }
    
    /***************************************/
    /********   MAIN KRUSKAL LOOP  *********/
    /***************************************/
    Iterate(it, edge_list){ // it -> Edge
        basic_step++;
        
        //iterate through city sets
        Iterate(Set1, city_sets){    // ite -> set of cities 
            lsi::iterator Set2;		//Set1 & Set2, sets to be united
            bool cont = false;

            //find city1 in the set
            si::iterator foundCity1 = (*Set1).find((*it)->GetCity1());
            if(foundCity1 != (*Set1).end()){	//city1 is found in Set1 now
            	//search for city2 in Set1
            	si::iterator foundCity2 = (*Set1).find((*it)->GetCity2());
            	if(foundCity2 == (*Set1).end()){ //if 2 cities are NOT in same set	
            		tree.push_back(*it);
            		
            		//find city2's set and unite them
            		Iterate(Set2, city_sets){
            			foundCity2 = (*Set2).find((*it)->GetCity2());
            			if(foundCity2 != (*Set2).end()){	//found the city2's set
                            (*Set1).insert(whole((*Set2))); //Unite sets
                            city_sets.erase(Set2); 
            				cont = true;    //continue edge iteration
            				break;  //break the set-list searching iteration
            			}
            		}
            	}
                else{   //if 2 cities are in the same set 
                    cont = true;    //continue edge iteration
                    break;  //break the set-list searching iteration
                }
            }               //if 2 cities are in same set *OR*
            if(cont == true){ // the edge is added to the Tree set
            	break;   // break the set-list iteration
            }           // continue edge iteration
        }
        if(city_sets.size() == 1){  //MST found
            cout << "Minimum spanning tree found!! " <<
                    "at " << basic_step << " steps." << endl;
            break;  //break the main loop
        }
    }
    if(city_sets.size() != 1){
        cout << "an MST is NOT found! " << endl;
    }
    
    return tree;
}
lE kruskal(lE& edge_list, msi& cities, bool printInfo){ //STEPS ARE SHOWN
    cout << endl << "Kruskal() is invoked. . . " << endl << endl;
    getchar();
    unsigned int basic_step = 0;
    cout << "Initial edges that will form an MST" << endl;
    edge_list.sort(CompareCost());  //sort the edge list
    print_list(edge_list, cities);
    getchar();
    print_list(edge_list);
    
    lE tree;    //tree, edge list
    lsi city_sets;   
    si node;    //int set (node, single city)
    Iterate(it, cities){    //make the singleton city sets
        node.insert(it->second);
        city_sets.push_back(node);
        node.clear();
    }
    
    cout << "Before the main loop " << endl;
    print_sets(city_sets);
    /***************************************/
    /********   MAIN KRUSKAL LOOP  *********/
    /***************************************/
    cout << "** STARTING MAIN LOOP **" << endl << endl;
    Iterate(it, edge_list){ // it -> Edge
        basic_step++;
        
        //iterate through city sets
        Iterate(Set1, city_sets){    // ite -> set of cities 
            lsi::iterator Set2;		//Set1 & Set2, sets to be united
            bool cont = false;

            cout << "Comparing Set1(w/ 1st element) " << *((*Set1).begin()) 
                    << "  with current_edge.city1 " << (*it)->GetCity1() <<
                    " and city2 " << (*it)->GetCity2() << endl;
            
            //find city1 in the set
            si::iterator foundCity1 = (*Set1).find((*it)->GetCity1());
            if(foundCity1 != (*Set1).end()){	//city1 is found in Set1 now
            	//search for city2 in Set1
            	si::iterator foundCity2 = (*Set1).find((*it)->GetCity2());
            	if(foundCity2 == (*Set1).end()){	//if 2 cities are NOT in same set	
        			cout << "cities are not in the same set" << endl;
                    cout << "pushing edge into tree" << endl;
            		tree.push_back(*it);
            		
            		//find city2's set and unite them
            		Iterate(Set2, city_sets){
            			foundCity2 = (*Set2).find((*it)->GetCity2());
            			if(foundCity2 != (*Set2).end()){	//found the city2's set
                            (*Set1).insert(whole((*Set2))); //Unite sets
                            city_sets.erase(Set2); 
            				cont = true;    //continue edge iteration
            				break;  //break the set-list searching iteration
            			}
            		}
                    cout << "All the sets after union" << endl;
                    print_sets(city_sets);
                    getchar();
            	}
                else{   //if 2 cities are in the same set 
                    cout << "2 cities are in the same set!" << endl;
                    cout << "Proceeding to next iteration" << endl << endl;
                    cont = true;    //continue edge iteration
                    break;  //break the set-list searching iteration
                }
            }
            if(cont == true){   //if 2 cities are in same set *OR*
            	break;  // the edge is added to the Tree set
            }           // continue edge iteration
        }
        if(city_sets.size() == 1){  //MST found
            cout << "All the sets after union" << endl;
            print_sets(city_sets);
            cout << "Minimum spanning tree found!! " <<
                    "at " << basic_step << " steps." <<  endl;
            break;  //break the main loop
        }
    }
    if(city_sets.size() != 1){
        cout << "All the sets after union" << endl;
        print_sets(city_sets);
        cout << "an MST is NOT found! " << endl;
    }
    
    return tree;
}
double kruskal_time(lE& edge_list, msi& cities){ //computes running time
    cout << "Calculating running time. . . " << endl;
    edge_list.sort(CompareCost());  //sort the edge list
    
    lE tree;    //tree, edge list
    lsi city_sets;   
    si node;    //int set (node, single city)
    Iterate(it, cities){    //make the singleton city sets
        node.insert(it->second);
        city_sets.push_back(node);
        node.clear();
    }
    
    const int DIVIDER = 10000000;
    time_t begin, total=0;
    for(int i=0 ; i<DIVIDER ; i++){
        begin = clock();
        /***************************************/
        /********   MAIN KRUSKAL LOOP  *********/
        /***************************************/
        Iterate(it, edge_list){ // it -> Edge

            //iterate through city sets
            Iterate(Set1, city_sets){    // ite -> set of cities 
                lsi::iterator Set2;		//Set1 & Set2, sets to be united
                bool cont = false;

                //find city1 in the set
                si::iterator foundCity1 = (*Set1).find((*it)->GetCity1());
                if(foundCity1 != (*Set1).end()){	//city1 is found in Set1 now
                    //search for city2 in Set1
                    si::iterator foundCity2 = (*Set1).find((*it)->GetCity2());
                    if(foundCity2 == (*Set1).end()){ //if 2 cities are NOT in same set	
                        tree.push_back(*it);

                        //find city2's set and unite them
                        Iterate(Set2, city_sets){
                            foundCity2 = (*Set2).find((*it)->GetCity2());
                            if(foundCity2 != (*Set2).end()){	//found the city2's set
                                (*Set1).insert(whole((*Set2))); //Unite sets
                                city_sets.erase(Set2); 
                                cont = true;    //continue edge iteration
                                break;  //break the set-list searching iteration
                            }
                        }
                    }
                    else{   //if 2 cities are in the same set 
                        cont = true;    //continue edge iteration
                        break;  //break the set-list searching iteration
                    }
                }               //if 2 cities are in same set *OR*
                if(cont == true){ // the edge is added to the Tree set
                    break;   // break the set-list iteration
                }           // continue edge iteration
            }
            if(city_sets.size() == 1){  //MST found
                break;  //break the main loop
            }
        }
        //since the provided list will form a connected graph...
        
        total += clock()-begin;
    }
    cout << "Completed in " << ((double)total * 1000)/DIVIDER << "ms" << endl;
    return ((double)total * 1000)/DIVIDER;
}