SQLINCLUDE = -rpath /usr/local/mysql-connector-c++-8.3/lib64 -I/usr/local/mysql-connector-c++-8.3/include/jdbc -L/usr/local/mysql-connector-c++-8.3/lib64 -lmysqlcppconn 

main: main.o server.o dbconnector.o
	g++ -std=c++17 -o main main.o server.o  $(SQLINCLUDE)

main.o: main.cpp
	g++ -std=c++17 -o main.o -c $(SQLINCLUDE) main.cpp

server.o: server.cpp
	g++ -std=c++17 -o server.o -c $(SQLINCLUDE) server.cpp 

dbconnector.o: dbConnector.cpp
	g++ -std=c++17 -o dbconnector.o -c $(SQLINCLUDE) dbConnector.cpp 
clean:
	rm -f *.o main
