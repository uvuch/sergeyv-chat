reader: main.o reader.o sharedNetwork.o
	g++ -std=c++17 -o reader sharedNetwork.o reader.o main.o

main.o: main.cpp
	g++ -std=c++17 -o main.o -c main.cpp
reader.o: reader.cpp
	g++ -std=c++17 -o reader.o -c reader.cpp

sharedNetwork.o: ../sharedNetwork.cpp
	g++ -std=c++17 -o sharedNetwork.o -c ../sharedNetwork.cpp

clean:
	rm -f *.o reader 
