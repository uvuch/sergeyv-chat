SQLINCLUDE = -rpath /usr/local/mysql-connector-c++-8.3/lib64 -I/usr/local/mysql-connector-c++-8.3/include/jdbc -L/usr/local/mysql-connector-c++-8.3/lib64 -lmysqlcppconn 

main: main.cpp server.o dbconnector.o
	g++ -std=c++17 -o main $(SQLINCLUDE) main.cpp server.o dbconnector.o

server.o: server.cpp dbconnector.o
	g++ -std=c++17 -o server.o -c $(SQLINCLUDE) dbconnector.o server.cpp 

dbconnector.o: dbConnector.cpp
	g++ -std=c++17 -o dbconnector.o -c $(SQLINCLUDE) dbConnector.cpp 

clean:
	rm -f *.o main
