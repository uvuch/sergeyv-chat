main: main.o server.o global.o
	g++ -std=c++17 -o main main.o server.o global.o

main.o: main.cpp
	g++ -std=c++17 -o main.o -c main.cpp

server.o: server.cpp
	g++ -std=c++17 -o server.o -c server.cpp 

global.o: global.cpp
	g++ -std=c++17 -o global.o -c global.cpp

clean:
	rm *.o
