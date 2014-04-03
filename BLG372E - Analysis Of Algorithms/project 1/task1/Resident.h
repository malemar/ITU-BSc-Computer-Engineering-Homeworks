/* 
 * File:   Resident.h
 * Author: varaquilex
 *
 * Created on March 17, 2013, 7:58 PM
 */

#ifndef RESIDENT_H
#define	RESIDENT_H

class Hospital;
class Resident {
public:
    //constructors & destructors
    Resident();
    Resident(const int, const int);
    Resident(const Resident& orig);
    virtual ~Resident();
    
    //getters, setters
    int getNumber() const;
    int* getPreferenceList() const;
    bool getAdmissionStatus() const;
    int getProposalCount() const;
    int* getCurrentPreference() const;
    void setAdmissionStatus(const bool);
    Hospital* getAdmittedHospital() const;
    void setAdmittedHospital(Hospital*);
    
    //member functions
    void addPreference(const int, const int);
    void incrementProposalCount();
    void incrementCurrentPreference();
    bool isAdmittedTo(Hospital*);
private:
    int number;     //resident number
    int proposalCount;
    int* preferenceList;    //not inverse, unlike Hospitals pref list
    bool admissionStatus;
    int* currentPreference;
    Hospital* admittedHospital;
};

#endif	/* RESIDENT_H */

