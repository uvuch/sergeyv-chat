#include "dbconnector.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cstdlib>
#include <mysql_connection.h>
#include <string>

// DBConnector *DBConnector::m_pInstance = nullptr;

/* DBConnector *DBConnector::instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new DBConnector;
  return m_pInstance;
}
*/

void DBConnector::shutdown() {
  if (con)
    delete con;
}

int DBConnector::connect(char *port, char *user, char *password) {
  try {
    driver = get_driver_instance();

    // connect("tcp://127.0.0.1:3306", "user", "password");
    std::string addr = "tcp://127.0.0.1:";
    addr += port;
    con = driver->connect(addr, user, password);
    con->setSchema("connections");
    stmt = con->createStatement();

    m_bRunning = true;

  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return -1;
  }

  return 0;
}

int DBConnector::addClientReader(int clientfd) {
  if (!m_bRunning)
    return EXIT_FAILURE;

  try {
    sql::ResultSet *res;
    res = stmt->executeQuery("INSERT into readersTable (clientfd) VALUES (" +
                             std::to_string(clientfd) + ")");

    return 0;

  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return -1;
  }
}

int DBConnector::addClientWriter(int clientfd) {
  if (!m_bRunning)
    return EXIT_FAILURE;

  try {
    sql::ResultSet *res;
    res = stmt->executeQuery("INSERT into writersTable (clientfd) VALUES (" +
                             std::to_string(clientfd) + ")");

    return 0;

  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return -1;
  }
}
