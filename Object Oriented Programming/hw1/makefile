oop1:	Actor.o	main.o	Movie.o
	g++ Actor.o main.o Movie.o -o oop1
Actor.o:	Actor.h	Actor.cpp
	g++ -c	Actor.cpp
main.o:	Actor.h	Movie.h	main.cpp
	g++ -c main.cpp
Movie.o:	Movie.h	Movie.cpp
	g++ -c 	Movie.cpp
