/*
*
* BLG252E
* 2012 Spring
* 1st Homework
*
*/
/*
* @Author
* Student Name: Volkan Ýlbeyli
* Student ID : 040100118
* Date: 11.03.2012
*/

#include "Actor.h"
#include <string>
#include <iostream>

using namespace std;

//Constructor of Actor with 3 parameters
Actor::Actor(const string nameInput, const int yearInput, const string nationalityInput){
	birth_year = yearInput;
	name = nameInput;
	nationality = nationalityInput;
	constructor_count++;
}

Actor::Actor(const Actor& a){
	name = a.name;
	birth_year = a.birth_year;
	nationality = a.nationality;

	constructor_count++;
}

void Actor::print() const {
	cout << endl << "Actor's Information" << endl;
	cout << "Name: " << name << endl;
	cout << "Birth year: " << birth_year << endl;
	cout << "Nationality: " << nationality << endl << endl;
	return;
}