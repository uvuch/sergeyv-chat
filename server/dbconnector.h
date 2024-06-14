#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class DBConnector {
private:
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  // sql::ResultSet *res;

  bool m_bRunning = false;

public:
  DBConnector() {}
  ~DBConnector() {}

  void shutdown();

  int connect(char *port, char *user, char *password);
  int addClientReader(int clientfd);
  int addClientWriter(int clientfd);
};

#endif // !DBCONNECTOR_H
