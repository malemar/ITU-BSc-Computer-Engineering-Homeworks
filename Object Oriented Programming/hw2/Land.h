/*
*
* BLG252E
* 2012 Spring
* 2nd Homework
*
*/
/*
* @Author
* Student Name: Volkan Ýlbeyli
* Student ID : 040100118
* Date: 17.04.2012
*/
#ifndef LAND_H
#define LAND_H
#include <string>
#include "Card.h"

using namespace std;

class Land : public Card{
public:
	Land(const string&, const string&);
	Land();		//randomly generates cards of the 5 types
	void print() const;
};


#endif