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
#include "Enchantment.h"
#include <iostream>

using namespace std;

Enchantment::Enchantment(const string& nameIn, const string& colorIn, int ap, int hp, Cost* costIn, const string& descr){
	name = nameIn;
	color = colorIn;
	attackPower = ap;
	hitPoints = hp;
	description = descr;

	//copying cost contents into class
	cost = new Cost(costIn->total(), costIn->white(), costIn->black(), costIn->blue(), costIn->green(), costIn->red());
}

Enchantment::~Enchantment(){
	delete cost;
}

bool Enchantment::isAffordable(const Land* land, const int size) const{
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
