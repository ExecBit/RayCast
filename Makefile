all:
	g++ -c MapObject.cpp
	g++ -c camera.cpp
	g++ -c main.cpp

	g++ main.o MapObject.o camera.o MapObject.hpp camera.hpp -o sfml-app -lsfml-graphics -lsfml-system -lsfml-window