main: main.cpp server.o 
	g++ -std=c++17 -o main  main.cpp server.o 

server.o: server.cpp
	g++ -std=c++17 -o server.o -c server.cpp 

clean:
	rm -f *.o main
