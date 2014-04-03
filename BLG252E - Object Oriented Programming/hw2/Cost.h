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
#ifndef COST_H
#define COST_H
#include "Card.h"
#include <string>

using namespace std;

class Cost{
public:
	Cost(int, int, int, int, int, int);	//total white black blue green red
	Cost(){}
	int total() const { return mana[0]; }
	int white() const { return mana[1]; }
	int black() const { return mana[2]; }
	int blue() const { return mana[3]; }
	int green() const { return mana[4]; }
	int red() const { return mana[5]; }

protected:
	int mana[6];
};

#endif
