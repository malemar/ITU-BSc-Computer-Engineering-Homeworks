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
#include <cstdlib>
#include <cstring>
#include "Resident.h"
#include "Hospital.h"
#include <stack>
#include <list>
#include <fstream>

using namespace std;

bool readFromFiles(const string, const string, const int, int*, int*);
void printMatch(Hospital*[5], Resident*[15]);   //prints if the sizes are less than or equal to 5,15 
void outputToFiles(char*, const int, const int, const int);
void cleanup(const int, const int);

Resident** residents;
Hospital** hospitals;

int main(int argc, char* argv[]) {
    int quota;
    int residentCount, hospitalCount;   //initialized in readFromFiles() function

    //check argument count
    if(argc < 4 || argc > 4){
        cout << "Invalid arguments, example: ./a.out data1_h1.txt data1_r1.txt 5" << endl;
        cout << "Note that resident and hospital numbers (h1,r1) should match." << endl;
        cout << "Returning from main() . . ." << endl;
        return -1;
    }
    quota = atoi(argv[3]);
    if(quota < 0){
        cout << "Quota can NOT be negative." << endl;
        cout << "Returning from main() . . ." << endl;
        return -1;
    }
        
    if(readFromFiles(argv[1], argv[2], quota, &hospitalCount, &residentCount) == false)
        return -1;  //error msg is printed in function
    
    cout << "HospitalCount: " << hospitalCount << "\t Resident count:" << residentCount << endl;
    /*************************************************************************************************/
    /**********************************  STABLE MATCH    *********************************************/
     /************************************************************************************************/
    //initialize free resident stack
    stack<Resident*> freeResidentStack;
    for(int i=residentCount-1 ; i>=0 ; i--){
        if(residents[i]->getAdmissionStatus() == false)
            freeResidentStack.push(residents[i]);
    }
    //print free resident stack
    /*cout << "\nfree resident stack: " << endl;
    for(int i=0 ; i<residentCount ; i++){
        cout << "Resident #" << freeResidentStack.top()->getNumber() << endl;
        freeResidentStack.pop();
    }*/
    int hospitalNumber;
    //algorithm
    while(!freeResidentStack.empty() ){ //if the resident proposed all the hospitals
        if(freeResidentStack.top()->getProposalCount() == hospitalCount){
            freeResidentStack.pop();    //set her unmatched
            continue;
        }
        hospitalNumber = *freeResidentStack.top()->getCurrentPreference();
        if(hospitals[hospitalNumber]->getQuota() != 0){ //if hospitals quota is not full (Hospital free)
            //RESIDENT OPERATIONS
            freeResidentStack.top()->incrementProposalCount();  //increment the proposing residents proposal count
            if(freeResidentStack.top()->getProposalCount() != hospitalCount)   //to avoid segmentation fault
               freeResidentStack.top()->incrementCurrentPreference();  //move pointer to the next preference
            freeResidentStack.top()->setAdmissionStatus(true);
            cout << "R# " << freeResidentStack.top()->getNumber() << " proposing(" << freeResidentStack.top()->getProposalCount() << ")" << endl;
            
            //HOSPITAL OPERATIONS
            hospitals[hospitalNumber]->addResident(freeResidentStack.top());
            cout << "Matched resident #" << freeResidentStack.top()->getNumber() << " with Hospital#" << hospitalNumber << " with preference order " << hospitals[hospitalNumber]->getPreferenceList()[0][freeResidentStack.top()->getNumber()] <<  " Current quota: " << hospitals[hospitalNumber]->getQuota() << endl;
            freeResidentStack.pop();    //remove the resident from the free resident stack
        }
        else if(hospitals[hospitalNumber]->getQuota() == 0){
            if(hospitals[hospitalNumber]->isAdmissable(freeResidentStack.top())){  //if the residents preference order is higher from anyone in the list
                //kick a resident out of the hospital and replace the proposing resident
                //RESIDENT OPERATIONS
                freeResidentStack.top()->incrementProposalCount();  //increment the proposing residents proposal count
                cout << "R# " << freeResidentStack.top()->getNumber() << " proposing(" << freeResidentStack.top()->getProposalCount() << ")" << endl;
                cout << "Replacing resident #" << freeResidentStack.top()->getNumber() << " with resident #" << hospitals[hospitalNumber]->findWeakest()->getNumber() << endl;
                if(freeResidentStack.top()->getProposalCount() != hospitalCount)   //to avoid segmentation fault
                   freeResidentStack.top()->incrementCurrentPreference();  //move pointer to the next preference
                freeResidentStack.top()->setAdmissionStatus(true);
                
                //HOSPITAL OPERATIONS
                hospitals[hospitalNumber]->addResident(freeResidentStack.top());
                cout << "Added resident #" << freeResidentStack.top()->getNumber() << " to hospital #" << hospitals[hospitalNumber]->getNumber() << endl;
                freeResidentStack.pop();
                freeResidentStack.push(hospitals[hospitalNumber]->findWeakest());   //resident with lowest preference order is pushed onto stack
                //cout << "pushed resident #" << freeResidentStack.top()->getNumber() << endl;
                //cout << "weakest is pushed to the stack: " << hospitals[hospitalNumber]->findWeakest()->getNumber() <<  endl;
                hospitals[hospitalNumber]->removeResident(freeResidentStack.top());
                freeResidentStack.top()->setAdmissionStatus(false); //removed resident's status = false
            }
            else{  //if the resident is not admissable, move to next preference
                freeResidentStack.top()->incrementProposalCount();  //increment the proposing residents proposal count
                cout << "R# " << freeResidentStack.top()->getNumber() << " proposing(" << freeResidentStack.top()->getProposalCount() << ")" << endl;
                cout << "Resident #" << freeResidentStack.top()->getNumber() << " is not admitted to hospital #" << hospitalNumber << endl;
                if(freeResidentStack.top()->getProposalCount() != hospitalCount){   //to avoid segmentation fault
                    freeResidentStack.top()->incrementCurrentPreference();  //move to next preference
                }
                else //if the resident can not get in any of the hospitals she wants, remove her from the stack
                    freeResidentStack.pop();
            }   
        }
    }
    cout << "Hospitals' resident lists after matching" << endl;
    for(int i=0 ; i<hospitalCount ; i++){
        hospitals[i]->printList();
    }
    if(hospitalCount <= 5 && residentCount <= 15)
        printMatch(hospitals, residents);   //nice output only for data1_h1 and data1_r1 files ;)
    
    outputToFiles(argv[1], quota, hospitalCount, residentCount);    //write output to files
    cout << "Output is written to file" << endl;
    cleanup(hospitalCount, residentCount);
    return 0;
}

void printMatch(Hospital* hospitals[5], Resident* residents[15]){
    //print resident choices
    cout << "\t\t";
    for(int i = 0 ; i<5 ; i++)
        cout << "Choice #" << i << "\t";
    cout << endl;
    for(int i=0 ; i<15 ; i++){
        cout << "Resident #" << i << "\t";
        for(int j=0 ; j<5 ; j++){
            if(residents[i]->isAdmittedTo(hospitals[residents[i]->getPreferenceList()[j]]))
                cout << "*H #" << residents[i]->getPreferenceList()[j] << "*\t\t";
            else
                cout << "H #" << residents[i]->getPreferenceList()[j] << "\t\t";
        }
        cout << endl;
    }
    cout << endl << endl;
    
    //hospitals
    FILE *fp;
    
    int num[5][15];
    
    //read hospitals
    fp = fopen("data1_h1.txt","r");
    if(fp == NULL)  //error check
        cout << "Error opening file: data1_h1.txt" << endl;
    
    //get hospitals' preference list in normal order for printing
    for(int i=0 ; i<5 ; i++){  //hospital number
        for(int j=0 ; j < 15 ; j++){    //preference order
            fscanf(fp, "%d ", &num[i][j]);    // num = resident# , j = order
        }
    }
    fclose(fp);
    
    cout << "Choice #\t";
    for(int i = 0 ; i<15 ; i++)
        cout << i << "\t";
    cout << endl;
    for(int i=0 ; i<5 ; i++){
        cout << "Hospital #" << i << "\t";
        for(int j=0 ; j<15 ; j++){
            //cout << "resident #" << residents[j]->getNumber() << endl;
            if(hospitals[i]->isResidentIn(num[i][j]))
                cout << "*R #" << num[i][j] << "*\t";
            else    
                cout << "R #" << num[i][j] << "\t";
        }
        cout << endl;
    }
}
bool readFromFiles(const string h, const string r, const int quota, int* h_count, int* r_count){
    //IMPORTANT: Note that the preference length of all of the hospitals in the 
    //given file will be equal to the first hospitals preference length.
    //In addition, Resident count will be the same as that preference length.
    //Please provide the 1st hospital carefully in order for the program to work
    FILE *fp;
    int num, lineCount;
    char c;
    //read hospitals
    fp = fopen(h.c_str(),"r");
    if(fp == NULL){  //error check
        cout << "Error opening file: " << h << endl;
        cout << "Returning. . ." << endl;
        return false;
    }
    
    /**********************************************************************/
    /*********************** IMPORTANT NOTE!!!!! **************************/
    /**********************************************************************/
    //when a file is created or edited in linux, the lineCount should start
    //from 0 in order to count the number of lines correctly. If you are going
    //to test the program in linux environment, MAKE SURE THE FILES ARE CREATED
    //OR SAVED IN LINUX PLATFORM. Windows based txt files will result in crashes.
    lineCount = 0;
    do{ //find the line count in the file = hospital count
        c = fgetc(fp);
        if(c == '\n') lineCount++;
    }
    while(c != EOF);
    fclose(fp);
    cout << "LINE COUNT " << lineCount << endl;
    ifstream file(h.c_str());
    
    //find the preference count
    int prefCount = 0;
    string line;
    if(file.is_open())
        getline(file, line);
    char * writable = new char[line.size() + 1];    //writable char[] for strtok funct
    copy(line.begin(), line.end(), writable);
    writable[line.size()] = '\0';   //append last character
    char* token = strtok(writable, " ");    //start tokenizing the line
    while(token){   //count the preferences
        prefCount++;
        token = strtok(NULL, " ");
    } prefCount--;
    
    //initialize hospitals & residents
    hospitals = new Hospital*[lineCount];
    for(int i=0 ; i<lineCount ; i++)    //quota, hospital number, pref length
        hospitals[i] = new Hospital(quota, i, prefCount);
    
    residents = new Resident* [prefCount];
    for(int i=0; i<prefCount ; i++)
        residents[i] = new Resident(i, lineCount);
    
    *r_count = prefCount;
    *h_count = lineCount;
    
    //read preferences from files
    //read hospitals
    fp = fopen(h.c_str(),"r");
    if(fp == NULL){  //error check
        cout << "Error opening file: " << h << endl;
        cout << "Returning. . ." << endl;
        return false;
    }
    //REVERSING THE PREFERENCE LIST (in addPreferenece() function)
    for(int i=0 ; i<lineCount ; i++){  //hospital number
        for(int j=0 ; j < prefCount ; j++){    //preference order
            fscanf(fp, "%d ", &num);    // num = resident# , j = order
            hospitals[i]->addPreference(num, j);
        }
    }
    fclose(fp);
    
    //read residents
    fp = fopen(r.c_str(),"r");
    if(fp == NULL){  //error check
        cout << "Error opening file: " << r << endl;
        cout << "Returning. . ." << endl;
        return false;
    }
    
    for(int i=0 ; i<prefCount ; i++){  //resident number
        for(int j=0 ; j < lineCount ; j++) {
            fscanf(fp, "%d ", &num);
            residents[i]->addPreference(j, num);
        }
    }
    fclose(fp);
    return true;
}
void outputToFiles(char* inputFileName ,const int quota, const int hospitalCount, const int residentCount){
    FILE* fp;
    char* ch = &inputFileName[0];   //for generating data1_S(?), input file name is read
    
    //for outputting, read the index
    while(*ch != '.') ch++; //get to the '.' in the input file name
    ch--;   //go back 1 step, which is where the index resides
    
    string outputFileName("data1_S");
    outputFileName += *ch;
    outputFileName += ".txt";
  
    fp = fopen(outputFileName.c_str(),"a");
    if(fp == NULL)  //check error
        cout << "Error opening file: " << outputFileName << endl;
    fprintf(fp, "Quota = %d\n", quota);
    for(int i=0 ; i<residentCount ; i++){
        if(residents[i]->getAdmissionStatus() != false)
            fprintf(fp, "%d %d\n", residents[i]->getNumber(), residents[i]->getAdmittedHospital()->getNumber());
        else
            fprintf(fp, "%d -\n", residents[i]->getNumber());
    }
    fprintf(fp, "\n\n");
    fclose(fp);
    return;
}
void cleanup(const int h_count, const int r_count){
    for(int i=0 ; i<r_count ; i++)
        delete residents[i];
    delete residents;
    for(int i=0 ; i<h_count ; i++)
        delete hospitals[i];
    delete hospitals;
    return;
}