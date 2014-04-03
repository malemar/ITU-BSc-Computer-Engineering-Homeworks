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
#ifndef CREATURE_H
#define CREATURE_H
#include <string>
#include "Card.h"
#include "Enchantment.h"
#include "Cost.h"
#include "Land.h"

using namespace std;

class Creature : public Land{
public:
	Creature(const string&, const string&, int, int, Cost*, const string&);// parameters : name, color, ap, hp, cost, description-flavor text
	Creature();
	Creature::~Creature();
	void print() const;
	bool canEnchantable(const Enchantment& enchIn) const;
	void operator+(Enchantment& enchIn);
	bool isAffordable(const Land*,const int) const;
	

protected:
	Enchantment* ench;
	string description;
	int hitPoint, attackPower;
	Cost* cost;

};

#endif