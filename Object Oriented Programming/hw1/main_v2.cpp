#include <iostream>
#include <string>
#include "Actor.h"
#include "Movie.h"

using namespace std;

#define SIZE 3

int Actor::constructor_count = 0;
int Actor::destructor_count = 0;
int Movie::constructor_count = 0;
int Movie::destructor_count = 0;

int main()
{
	{
		// sets actor name,birth year and nationality
		Actor a1("Leonardo DiCapr", 1974, "American");
		string n("Leonardo Di Caprio");
		// sets name of a1
		a1.set_name(n);
		// prints actor's information
		a1.print();
		// create new actor and initiliaze with a1's information
		Actor a2 = a1;
		a2.print();
		Actor a3("Kate Winslet", 1975, "American");
		// sets film name, year, duration, number of casts, country, oscar information and imdb points
		Movie m1("Titanic", 1997, 180, 5, "United States", true, 7.1);   
		// tries to add a cast (a1) to m1
		if(m1.add_cast(a1))  
			cout << "Cast has been added" << endl;
		else 
			cout << "Max cast has been reached, you can not add cast.." << endl;
		if(m1.add_cast(a3))
			cout << "Cast has been added" << endl;
		else 
			cout << "Max cast has been reached, you can not add cast.." << endl;
		// print m1's information
		m1.print();
		/* create a new movie and initiliaze with m1's information */
		Movie m2(m1);  
		m2.print();
		Movie *mp;
		/* default constructor of movie will ask user input on the film and assigns initial values to name of the film, year,
		duration, number of casts,country, oscar status and imdb rating */
		mp = new Movie[SIZE];
		Actor a4("Sam Worthington", 1976, "British");
		Actor a5("Actor Name1", 1975, "Australian");
		Actor a6("Actor Name2", 1980, "American");
		// add casts to movies
		mp[0].add_cast(a4);
		mp[1].add_cast(a5);
		mp[2].add_cast(a6);
		// find these actors' films and prints these films to the screen
		find_actors_movie(mp,"Sam Worthington");
		find_actors_movie(mp,"Actor Name2");
		// find the movie with the highest imdb rating and prints that movie to the screen
		find_best_movie(mp);
		delete[] mp;
	}
	/* printing static counters to the screen */
	cout << "Constructor of Actor has been called " << Actor::get_counter_const() << " times." << endl;
	cout << "Destructor of Actor has been called " << Actor::get_counter_dest() << " times." << endl;
	cout << "Constructor of Movie has been called " << Movie::get_counter_const() << " times." << endl;
	cout << "Destructor of Movie has been called " << Movie::get_counter_dest() << " times." << endl;
	return 0;
}
