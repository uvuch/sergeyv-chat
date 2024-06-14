#include "dbconnector.h"
#include <cstdlib>
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

int DBConnector::connect(const char *port, const char *user,
                         const char *password) {
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
    if (res)
      delete res;

    res = stmt->executeQuery("INSERT into readersTable (clientfd) VALUES (" +
                             std::to_string(clientfd) + ")");

    delete res;

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
    if (res)
      delete res;

    res = stmt->executeQuery("INSERT into writersTable (clientfd) VALUES (" +
                             std::to_string(clientfd) + ")");

    delete res;
    return 0;

  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return -1;
  }
}

sql::ResultSet *DBConnector::getClientReaders() {
  try {
    if (res)
      delete res;

    res = stmt->executeQuery("SELECT * from readersTable AS _message");

    return res;

  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return nullptr;
  }
}

sql::ResultSet *DBConnector::getClientWriters() {
  try {
    if (res)
      delete res;

    res = stmt->executeQuery("SELECT * from writersTable AS _message");

    return res;

  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return nullptr;
  }
}

int DBConnector::removeClientReader(int clientfd) {
  try {
    if (res)
      delete res;

    // Just assuming it will succeed right now;
    res = stmt->executeQuery("DELETE FROM readersTable VALUE clientfd='" +
                             std::to_string(clientfd) + "'");

    return 0;

  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return -1;
  }
}

int DBConnector::removeClientWriter(int clientfd) {
  try {
    if (res)
      delete res;

    // Just assuming it will succeed right now;
    res = stmt->executeQuery("DELETE FROM readersTable VALUE clientfd='" +
                             std::to_string(clientfd) + "'");

    return 0;
  } catch (sql::SQLException &e) {
    std::cout << "SQLException in " << __FILE__ << std::endl;
    std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
    std::cout << "# ERR: " << e.what() << std::endl;
    std::cout << "SQLState: " << e.getSQLState() << " )" << std::endl;

    return -1;
  }
}
