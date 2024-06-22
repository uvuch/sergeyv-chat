main: main.cpp client.o reader.o
	g++ -std=c++17 -o main main.cpp client.o reader.o

reader.o: reader.cpp client.o
	g++ -std=c++17 -o reader.o -c reader.cpp client.o

client.o: ../client.cpp 
	g++ -std=c++17 -o client.o -c ../client.cpp

clean:
	rm -f *.o
