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

#ifndef MOVIE_H
#define MOVIE_H
#include <string>
#include "Actor.h"

class Movie{
public:
	Movie();
	Movie(const string, const int, const int, const int, const string, const bool, const double);
	Movie(const Movie& m);
	bool add_cast(const Actor&);
	void print() const;
	double imdb() const {return imdb_rating;}
	int actorNo() const {return actor_no;}
	Actor* actors() const {return actor;}
	static int get_counter_const() {return constructor_count;}
	static int get_counter_dest() {return destructor_count;} 
	~Movie();
private:
	Actor* actor;
	std::string name, country;
	int year, actor_no, duration, actor_count;	//actor_no is the number of actors of the movie
	bool oscar;									//while actor_count is the number of actors who are
	double imdb_rating;							//signed in to the actor array of the movie
	static int constructor_count, destructor_count;
};

#endif