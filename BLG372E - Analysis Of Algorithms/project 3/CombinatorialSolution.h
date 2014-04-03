/* 
 * File:   CombinatorialSolution.h
 * Author: Volkan İlbeyli
 *
 * Created on April 28, 2013, 12:57 AM
 */

#ifndef COMBINATORIALSOLUTION_H
#define	COMBINATORIALSOLUTION_H

#include "BigUnsignedInteger.h"
#include <vector>

using namespace std;

class CombinatorialSolution {
public:
    //constructors & destructors
    CombinatorialSolution();
    CombinatorialSolution(const CombinatorialSolution& orig);
    CombinatorialSolution(unsigned const int, unsigned const int);
    virtual ~CombinatorialSolution();

    //getters & setters

    //member functions
    void print() const;
private:
    vector< vector<BigUnsignedInteger> > triangle;
};

#endif	/* COMBINATORIALSOLUTION_H */

