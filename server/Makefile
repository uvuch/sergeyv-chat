main: main.o server.o
	g++ -std=c++17 -o main main.o server.o

main.o: main.cpp
	g++ -std=c++17 -o main.o -c main.cpp

server.o: server.cpp
	g++ -std=c++17 -o server.o -c server.cpp 

clean:
	rm -f *.o main
