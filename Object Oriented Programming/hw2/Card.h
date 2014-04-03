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

#include <string>
#ifndef CARD_H
#define CARD_H
#include "Cost.h"
using namespace std;

//base class for others
class Card{	
public:
	virtual void print() const = 0;
	Card(const string, const string);
	Card(){}
	string getColor() const { return color; }
	string getName() const { return name; }

protected:
	string name;
	string color;
};

#endif