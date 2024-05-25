reader: reader.o sharedNetwork.o
	g++ -std=c++17 -o reader sharedNetwork.o reader.o 

reader.o: reader.cpp
	g++ -std=c++17 -o reader.o -c reader.cpp

sharedNetwork.o: ../sharedNetwork.cpp
	g++ -std=c++17 -o sharedNetwork.o -c ../sharedNetwork.cpp

clean:
	rm *.o
