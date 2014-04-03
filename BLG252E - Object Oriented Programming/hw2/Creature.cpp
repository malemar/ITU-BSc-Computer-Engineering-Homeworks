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

#include "Creature.h"
#include <iostream>

using namespace std;

Creature::Creature(){	//it is always good to have pointers at NULL
	ench = NULL;		//when they are not assigned.
	cost = NULL;
}

Creature::Creature(const string& nameIn, const string& colorIn, int ap, int hp, Cost* costIn, const string& descr){
	// parameters : name, color, ap, hp, cost, description-flavor text
	name = nameIn;
	color = colorIn;
	hitPoint = hp;
	attackPower = ap;
	description = descr;
	ench = NULL;

	//copying cost contents into class
	cost = new Cost(costIn->total(), costIn->white(), costIn->black(), costIn->blue(), costIn->green(), costIn->red());
}

Creature::~Creature(){
	delete cost;
}

void Creature::print() const{
	cout << endl << "Type: Creature" << endl;
	cout << "Name: " << name << endl;
	cout << "Color: " << color << endl;
	cout << "Attack Power: " << attackPower << endl;
	cout << "Hit Points: " << hitPoint << endl;
	cout << "Cost: Total " << cost->total() << " mana cards, "
		<< cost->white() << " white, "
		<< cost->black() << " black, "
		<< cost->blue() << " blue, "
		<< cost->green() << " green, "
		<< cost->red() << " red. " << endl;

	cout << "Description: " << description << endl;
	cout << "Enchantment: ";
	if(ench != NULL)
		ench->print();
	else
		cout << "None" << endl;

}

bool Creature::canEnchantable(const Enchantment& enchIn) const{
	if(color == enchIn.getColor() && ench == NULL)
		return true;
	else
		return false;
}

void Creature::operator+(Enchantment& enchIn){
	if(ench == NULL){		//I designed here not dynamic since no enchantment
		ench = &enchIn;		//objects are deleted like cost objects in main.cpp
		hitPoint += ench->getHp();
		attackPower += ench->getAp();
	}
	else
		cout << "Invalid move: Creature already has an enchantment card on!" << endl;

}

bool Creature::isAffordable(const Land* land, const int size) const{
	int wh, bla, blu, gr, red;
	wh = bla = blu = gr = red = 0;

	if(size < cost->total())	return false;
	else{
		for(int i=0 ; i < size ; i++){
			if(land[i].getColor() == "white")
				wh++;
			else if(land[i].getColor() == "black")
				bla++;
			else if(land[i].getColor() == "blue")
				blu++;
			else if(land[i].getColor() == "green")
				gr++;
			else if(land[i].getColor() == "red")
				red++;
		}

		if(wh < cost->white() || bla < cost->black() ||
			blu < cost->blue() || gr < cost->green() ||
			red < cost->red())
			return false;
		else return true;
	}
}
	