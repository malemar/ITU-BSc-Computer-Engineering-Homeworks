/* 
 * File:   BigUnsignedInteger.h
 * Author: Volkan İlbeyli
 *
 * Created on April 25, 2013, 10:14 PM
 */

#ifndef BIGUNSIGNEDINTEGER_H
#define	BIGUNSIGNEDINTEGER_H

#include <string>
#include <vector>

using namespace std;

class BigUnsignedInteger {
public:
    //constructors & destructors
    BigUnsignedInteger();
    BigUnsignedInteger(const BigUnsignedInteger& orig);
    virtual ~BigUnsignedInteger();

    //getters & setters
    vector<unsigned int> getValue() const { return value; }
    void setValue(vector<unsigned int> value) { this->value = value; }
    
    //operators
    BigUnsignedInteger operator=(const unsigned int&);
    BigUnsignedInteger operator+(const BigUnsignedInteger&);
    BigUnsignedInteger operator+=(const BigUnsignedInteger&);
    BigUnsignedInteger operator+(const unsigned int&);
    BigUnsignedInteger operator+=(const unsigned int&);
    BigUnsignedInteger operator*(const BigUnsignedInteger&);
    BigUnsignedInteger operator*=(const BigUnsignedInteger&);
    BigUnsignedInteger operator*(const unsigned int&);
    BigUnsignedInteger operator*=(const unsigned int&);
    BigUnsignedInteger operator-(const unsigned int&); 
    BigUnsignedInteger operator-=(const unsigned int&);
    BigUnsignedInteger operator-(const BigUnsignedInteger&);
    BigUnsignedInteger operator-=(const BigUnsignedInteger&);
    BigUnsignedInteger operator/(const unsigned int&);
    BigUnsignedInteger operator/=(const unsigned int&);
    BigUnsignedInteger operator/(const BigUnsignedInteger&);
    BigUnsignedInteger operator/=(const BigUnsignedInteger&);
    bool operator==(const BigUnsignedInteger&);
    bool operator==(const unsigned int&);
    bool operator<(const BigUnsignedInteger&);
    bool operator<(const unsigned int&);
    bool operator<=(const BigUnsignedInteger&);
    bool operator<=(const unsigned int&);
    BigUnsignedInteger operator>>(const unsigned int);
    BigUnsignedInteger operator<<(const unsigned int);
    
    //member functions
    void print() const;
    unsigned int length() const;
    unsigned long long val() const;

private:
    vector<unsigned int> value;
};

#endif	/* BIGUNSIGNEDINTEGER_H */

