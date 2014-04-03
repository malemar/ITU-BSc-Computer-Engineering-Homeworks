/* 
 * File:   Hospital.h
 * Author: varaquilex
 *
 * Created on March 17, 2013, 7:56 PM
 */

#ifndef HOSPITAL_H
#define	HOSPITAL_H

#include "Resident.h"
#include <list>

using namespace std;

class Hospital {
public:
    //constructors & destructors
    Hospital();
    Hospital(const int, const int, const int);
    Hospital(const Hospital& orig);
    virtual ~Hospital();
    
    //getters & setters
    int getNumber() const;
    int getQuota() const;
    int** getPreferenceList();
    
    //member functions
    void addPreference(const int, const int);
    void addResident(Resident* resident);
    bool isAdmissable(Resident* resident);
    Resident* findWeakest();
    void removeResident(Resident* resident);
    void printList();
    bool isResidentIn(int);
private:
    int number; //hospital number
    int* preferenceList[2]; //1st row: preference order ; 2nd row: admission status(NOT USED, initial design.)
    int quota;
    list<Resident*> admittedResidents;
};

#endif	/* HOSPITAL_H */

