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

#ifndef ACTOR_H
#define ACTOR_H
#include <string>

using namespace std;

class Actor{
public:
	Actor(const string nameInput, const int yearInput, const string nationalityInput);
	Actor(const Actor&);
	Actor(){}	//defined but not implemented due to compilation on linux
	void set_name(const string nameInput){	name = nameInput;	return; }
	void print() const;
	string actorName() const {return name;}
	static int get_counter_const() {return constructor_count;}
	static int get_counter_dest() {return destructor_count;} 
private:
	string name;
	int birth_year;
	string nationality;
	static int constructor_count, destructor_count;
};

#endif
