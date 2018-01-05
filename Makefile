#declare the variable
CC=g++

#this target will compile all the files

all: smartcity

CFLAGS = -c -Wall

smartcity: Main.o Point.o Square.o Event.o Utility.o UtilityDecision.o Users.o
		$(CC) Main.o Point.o Square.o Event.cpp Utility.o UtilityDecision.o Users.o -o smartcity.out

Main.o: Main.cpp
		$(CC) $(CFLAGS) Main.cpp -g

Point.o: Point.cpp
		$(CC) $(CFLAGS) Point.cpp -g 

Square.o: Square.cpp
		$(CC) $(CFLAGS) Square.cpp -g

Event.o: Event.cpp
		$(CC) $(CFLAGS) Event.cpp -g

Utility.o: Utility.cpp
		$(CC) $(CFLAGS) Utility.cpp -g

UtilityDecision.o: UtilityDecision.cpp
		$(CC) $(CFLAGS) UtilityDecision.cpp -g


Users.o: Users.cpp
		$(CC) $(CFLAGS) Users.cpp -g

clean: 
		rm -rf *o smartcity


