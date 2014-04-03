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

#include "Movie.h"
#include <iostream>

using namespace std;

Movie::Movie(){
	string dummy;		//dummy variable for catching \n character

	//here getline catches \n to avoid the failure of getting films name input
	getline(cin,dummy);
	cout << "Please enter film's name: ";
	getline(cin,name);
	//cin.ignore(250,'\n');
	cout << "Please enter film's year: ";
	cin >> year;
	cout << "Please enter film's duration (min): ";
	cin >> duration;
	cout << "Please enter number of actors: ";
	cin >> actor_no;
	//here getline catches \n to avoid the failure of getting films country input
	getline(cin,dummy);
	cout << "Please enter film's country: ";
	getline(cin,country);
	//cin.ignore(250,'\n');
	cout << "Does the film have Oscar (0 or 1): ";
	cin >> oscar;
	cout << "Please enter imdb rating of the film:";
	cin >> imdb_rating;
	
	actor_count = 0;
	actor = new Actor[actor_no];
	constructor_count++;
	cout << "Film added!" << endl;
}

Movie::Movie(const string nameInput, const int yearInput, const int durationInput, const int castInput, const string countryInput, const bool oscarInput, const double imdbInput){
	name = nameInput;
	year = yearInput;
	duration = durationInput;
	actor_no = castInput;
	country = countryInput;
	oscar = oscarInput;
	imdb_rating = imdbInput;

	actor_count = 0;
	constructor_count++;
	actor = new Actor[actor_no];
}

Movie::Movie(const Movie& m){
	name = m.name;
	country = m.country;
	duration = m.duration;
	year = m.year;
	actor_no = m.actor_no;
	actor_count = m.actor_count;
	oscar = m.oscar;
	imdb_rating = m.imdb_rating;

	actor = new Actor[actor_no];
	for(int i=0 ; i < m.actor_count ; i++)
		actor[i] = m.actor[i];

	constructor_count++;
}

void Movie::print() const{
	cout << endl << "Movie's Information" << endl;
	cout << "Name: " << name << endl;
	cout << "Year: " << year << endl;
	cout << "Duration: " << duration << endl;
	cout << "# of actors: " << actor_count << endl;
	cout << "Country: " << country << endl;
	cout << "Oscar: " << oscar << endl;
	cout << "IMDb Rating: " << imdb_rating << endl;
	cout << endl << "Movie's actor list";
	for(int i=0 ; i < actor_count ; i++)	actor[i].print(); 
	cout << endl;
	return;
}

bool Movie::add_cast(const Actor& a){
	if(actor_count == actor_no)		return false;
	else{
		actor[actor_count] = a;
		actor_count++;
		return true;
	}
}

Movie::~Movie(){
	
	destructor_count++;
	delete []actor;
}
