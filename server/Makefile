main: main.o server.o dbconnector.o
	g++ -std=c++17 -o main main.o server.o -L/usr/local/mysql-connector-c++-8.3/lib64 -lmysqlcppconn 

main.o: main.cpp
	g++ -std=c++17 -o main.o -c main.cpp

server.o: server.cpp
	g++ -std=c++17 -o server.o -c server.cpp 

dbconnector.o: dbConnector.cpp
	g++ -std=c++17 -o dbconnector.o -c -rpath /usr/local/mysql-connector-c++-8.3/lib64 -I/usr/local/mysql-connector-c++-8.3/include/jdbc dbConnector.cpp 
clean:
	rm -f *.o main
