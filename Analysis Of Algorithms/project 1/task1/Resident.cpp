/* 
 * File:   Resident.cpp
 * Author: varaquilex
 * 
 * Created on March 17, 2013, 7:58 PM
 */

#include "Resident.h"
#include "Hospital.h"
#include <iostream>

using namespace std;

//constructor & destructors
Resident::Resident() {
}

Resident::Resident(const Resident& orig) {
}

Resident::~Resident() {
    delete this->preferenceList;
}

Resident::Resident(const int number, const int preferenceLength){
    this->number = number;
    this->proposalCount = 0;
    this->preferenceList = new int[preferenceLength];
    this->admissionStatus = false;
    this->currentPreference = &this->preferenceList[0];
    this->admittedHospital = NULL;
}

//getters & setters
int Resident::getNumber() const { return number; }
int* Resident::getPreferenceList() const { return preferenceList; }
bool Resident::getAdmissionStatus() const { return admissionStatus; }
int Resident::getProposalCount() const { return proposalCount; }
int* Resident::getCurrentPreference() const {return currentPreference; }
void Resident::setAdmissionStatus(const bool status){ admissionStatus = status; return; }
Hospital* Resident::getAdmittedHospital() const { return admittedHospital;}
void Resident::setAdmittedHospital(Hospital* hospital) { admittedHospital = hospital; }

//member functions
void Resident::addPreference(const int order, const int hospitalNumber){
    this->preferenceList[order] = hospitalNumber;
    return;
}
void Resident::incrementProposalCount(){ this->proposalCount++; }
void Resident::incrementCurrentPreference(){ this->currentPreference++; }
bool Resident::isAdmittedTo(Hospital* hospital){
    if(hospital->isResidentIn(this->getNumber()))
        return true;
    else
        return false;
}