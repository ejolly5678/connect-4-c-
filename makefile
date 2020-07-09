C4 : main.o 
	g++ -o C4 main.o -pthread
main.o: main.cpp
	g++ -c -std=c++11 main.cpp
