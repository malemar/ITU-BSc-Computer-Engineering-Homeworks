/* 
 * File:   Hospital.cpp
 * Author: varaquilex
 * 
 * Created on March 17, 2013, 7:56 PM
 */

#include "Hospital.h"
#include <iostream>

using namespace std;

//constructor & destructors
Hospital::Hospital() {
}

Hospital::Hospital(const Hospital& orig) {
}

Hospital::~Hospital() {
    delete this->preferenceList[0];
    delete this->preferenceList[1];
}

Hospital::Hospital(const int quota, const int number, const int preferenceLength) {
    this->quota = quota;
    this->number = number;  //hospital number
    //this->admittedResidents = new list<Resident*>;   //admission list with length, quota
    this->preferenceList[0] = new int[preferenceLength];    //resident order row
    this->preferenceList[1] = new int [preferenceLength];   //admission status row
    for(int i=0 ; i<preferenceLength ; i++) //initialize admission status to 0;
        this->preferenceList[1][i] = 0;
}

//getters & setters
int Hospital::getNumber() const { return number; }
int Hospital::getQuota() const { return quota; }
int** Hospital::getPreferenceList() { return this->preferenceList; } 


//member functions
void Hospital::addPreference(const int residentNumber, const int order){
    this->preferenceList[0][residentNumber] = order;
}
void Hospital::addResident(Resident* resident){
    this->admittedResidents.push_front(resident);
    this->preferenceList[1][resident->getNumber()] = 1; //also for showing in preference list's admission status(NOT USED)
    quota--;
    resident->setAdmittedHospital(this);
    return;
}
bool Hospital::isAdmissable(Resident* resident){  //returns true if the suggested resident has a higher order in pref list
    for(list<Resident*>::iterator it = this->admittedResidents.begin() ; it != this->admittedResidents.end() ; it++){
        //cout << "is admis resident #" << (*it)->getNumber() << " with pref order " << this->preferenceList[0][(*it)->getNumber()] << " vs " <<  this->preferenceList[0][resident->getNumber()] <<  endl;
        if(this->getPreferenceList()[0][resident->getNumber()] < this->getPreferenceList()[0][(*it)->getNumber()])
            return true;
    }
    //cout << "returned" << endl;
    return false;
}
Resident* Hospital::findWeakest(){  //returns the admitted resident with the lowest preference order
    Resident* weakest(this->admittedResidents.front());
    for(list<Resident*>::iterator it = this->admittedResidents.begin() ; it != this->admittedResidents.end() ; it++){
        //cout << "finding weakest: resident #" << (*it)->getNumber() << " with pref order" << this->preferenceList[0][(*it)->getNumber()] << endl;
        if(this->preferenceList[0][(*it)->getNumber()] > this->preferenceList[0][weakest->getNumber()])
            weakest = *it;
    }
    //cout << "weakest: #" << weakest->getNumber() << endl;
    return weakest;
}
void Hospital::removeResident(Resident* resident){
    this->getPreferenceList()[1][resident->getNumber()] = 0;
    this->admittedResidents.remove(resident);
    quota++;
    return;
}
void Hospital::printList(){
    cout << "Hospital #" << this->getNumber() << " has quota: " << this->getQuota() << endl; 
    for(list<Resident*>::iterator it = this->admittedResidents.begin() ; it != this->admittedResidents.end() ; it++)
        cout << "resident #" << (*it)->getNumber() << endl;
}
bool Hospital::isResidentIn(int resident){
    for(list<Resident*>::iterator it = this->admittedResidents.begin() ; it != this->admittedResidents.end() ; it++){
        if(resident == (*it)->getNumber())
            return true;
    }
    return false;
}
