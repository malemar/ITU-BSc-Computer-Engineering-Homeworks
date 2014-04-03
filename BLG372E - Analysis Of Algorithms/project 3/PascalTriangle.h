/* 
 * File:   PascalTriangle.h
 * Author: Volkan İlbeyli
 *
 * Created on April 27, 2013, 8:01 PM
 */

#ifndef PASCALTRIANGLE_H
#define	PASCALTRIANGLE_H

#include <vector>
#include "BigUnsignedInteger.h"

using namespace std;

class PascalTriangle {
public:
    //constructors & destructors
    PascalTriangle();
    PascalTriangle(const unsigned int, const unsigned int);
    PascalTriangle(const PascalTriangle& orig);
    virtual ~PascalTriangle();

    //getters & setters

    //member functions
    void print() const;
    
private:
    vector< vector<BigUnsignedInteger> > triangle;
};

#endif	/* PASCALTRIANGLE_H */

