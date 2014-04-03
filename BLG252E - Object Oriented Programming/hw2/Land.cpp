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
#include "Land.h"
#include "Creature.h"
#include <iostream>

using namespace std;

Land::Land(const string& colorIn, const string& nameIn){
	color = colorIn;
	name = nameIn;
}

//Default constructor for random card generation
Land::Land(){
	int random;

	random = rand()%5;
	switch(random){
	case 0:
		name = "island";
		color = "blue";
		break;
	case 1:
		name = "swamp";
		color = "black";
		break;
	case 2:
		name = "forest";
		color = "green";
		break;
	case 3:
		name = "mountain";
		color = "red";
		break;
	case 4:
		name = "plains";
		color = "white";
		break;
	}

}

void Land::print() const{
	cout << endl << "Name: " << name << endl;
	cout << "Color: " << color << endl;
}

