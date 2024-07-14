writer: main.cpp client.o writer.o
	g++ -std=c++17 -o writer main.cpp client.o writer.o

reader.o: writer.cpp 
	g++ -std=c++17 -o writer.o -c writer.cpp 

client.o: ../client.cpp 
	g++ -std=c++17 -o client.o -c ../client.cpp

clean:
	rm -f *.o writer 
