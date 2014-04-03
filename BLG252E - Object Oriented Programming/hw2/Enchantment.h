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
#ifndef ENCHANTMENT_H
#define ENCHANTMENT_H
#include <string>
#include "Card.h"
#include "Cost.h"
#include "Land.h"
using namespace std;

class Enchantment : public Land{
public:
	Enchantment(const string&, const string&, int, int, Cost*, const string&);
	Enchantment(){}
	~Enchantment();
	int getAp() const { return attackPower; }
	int getHp() const { return hitPoints; }
	bool isAffordable(const Land* land, const int) const;
protected:
	string description;
	int attackPower, hitPoints;
	Cost* cost;
};

#endif