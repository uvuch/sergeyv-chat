reader: reader.cpp client.o
	g++ std=c++17 -o reader reader.cpp client.o

writer: writer.cpp client.o
	g++ std=c++17 -o writer writer.cpp client.o

client.o: client.cpp
	g++ std=c++17 -o client.o -c client.cpp

clean:
	rm -f *.o main
