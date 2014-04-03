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

#include <iostream>
#include <string>
#include <ctime>
#include "Land.h"
#include "Cost.h"
#include "Creature.h"
#include "Enchantment.h"

using namespace std;

int main(){
	srand((unsigned)time(NULL));

	Land l1("black", "snow covered swamp"); 
	Land l2("blue", "island"); 
	l1.print();
	// for the following array, default constructor is called 5 times 
	// let’s randomly initialize land cards in the default constructor srand called at the beginning of the code 
	Land l3[5];
	// cost object: we need total 3 manas, one of them must be 
	// red mana (the last one) color of other two manas are unimportant 
	// order of the parameters: total, white, blue, black, green, red 
	Cost *cost = new Cost(3,0,0,0,0,1);
	// parameters : name, color, ap, hp, cost, description-flavor text 
	Creature c1("anaba shaman", "red", 2, 2, cost, "just try taking this bull by the horns."); 
	delete cost; 
	cost = new Cost(5,1,0,0,0,0); 
	Creature c2("angel of mercy","white",3,3, cost,"when angel of mercy enters the battlefield, you gain 3 life."); 
	delete cost; 
	cost = new Cost(1,0,0,0,0,1); 
	Enchantment e("firebreathing", "red", 1,0, cost,"enchanted creature gets +1/+0 until end of turn.");
	// when a creature is enchanted, update its attack power and hit point 
	if (c2.canEnchantable(e)){ 
		cout << e.getName() << " is enchantable for " << c2.getName() << endl; 
		c2 + e; 
	} else if (c1.canEnchantable(e)){ 
		cout << e.getName() << " is not enchantable for " << c2.getName() << endl; 
		cout << e.getName() << " is enchantable for " << c1.getName() << endl; 
		c1 + e; 
	}
	else 
		cout << e.getName() << " can’t be enchanted to any creature!" << endl;

	if (c1.isAffordable(l3,5)) // if you look closely, you can see that 5 is size :) 
		cout << c1.getName() << " is affordable according to land deck l3 " << endl; 
	if (e.isAffordable(l3,5)) 
		cout << e.getName() << " is affordable according to land deck l3" << endl;
	// show time 
	Land *l4[8]; 
	for (int i=0; i<5; i++) 
		l4[i]=&l3[i]; 
	l4[5]=&c1; 
	l4[6]=&c2; 
	l4[7]=&e;
	for (int i=0; i<8; i++) 
		l4[i]->print();
	
	fflush(stdin);
	getchar();
	return 0;
}