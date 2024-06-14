#include "server.h"
#include "dbconnector.cpp"
#include "dbconnector.h"
#include <arpa/inet.h>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <utility>

#define MAXLINE 100
#define MAXCONNECTIONS 100

Server *Server::m_pInstance = nullptr;
volatile bool Server::m_bQuitCommand = false;
int Server::serverSocket = 0;

Server *Server::instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Server;

  return m_pInstance;
}

void Server::shutdown() {
  if (m_pInstance != nullptr)
    delete m_pInstance;
}

void Server::stop(int sig) {
  m_bQuitCommand = true;
  close(serverSocket);
}

void Server::run(int port) {
  if (signal(SIGINT, Server::stop) == SIG_ERR) {
    std::cout
        << "Server initialization error (signal handler could not be assigned)!"
        << std::endl;
    return;
  }

  std::cout << "Starting server listening on a port " << port << std::endl;
  std::cout << "Max connections: " << MAXCONNECTIONS << std::endl;

  isChild = false;

  // Get a server socket
  serverSocket = create_server(port);
  if (serverSocket == -1) {
    return;
  }

  int uid = getuid();
  setgid(uid);

  // Processing requests
  int clientfd = 0;
  while (!m_bQuitCommand && !isChild) {
    clientfd = accept_connections(serverSocket);
    if (clientfd == -1) {
      m_bQuitCommand = true;
      break;
    }

    // Child fork, true if child
    // Adds to procChildren at the same time
    if (Fork(clientfd)) {
      isChild = true;
      setgid(uid);
    }
  }

  if (isChild) {
    dbconn = new DBConnector;

    const char *port = "54321";
    const char *username = "sergey";
    const char *password = "password123";

    dbconn->connect(port, username, password);

    char buf[MAXLINE];
    memset(&buf, 0, MAXLINE);

    int bytesSent = 0;

    while (!m_bQuitCommand) {
      bytesSent = receiveMessage(clientfd, (char *)&buf);
      if (bytesSent == -1) {
        m_bQuitCommand = true;
        break;
      }

      spreadMessage((char *)&buf, bytesSent);
    }

    // Connetion over
    delete dbconn;
  }

  close(serverSocket);

  if (!isChild) {
    // Send sigint to children
    kill(0, SIGINT);

    int status, pid = 0;
    while ((pid = wait(&status)) > 0)
      if (WIFEXITED(status))
        std::cout << "Culled procChild -> " << pid << std::endl;

    // delete procChildren;
  }

  std::cout << "\nServer stopped" << std::endl;
}

int Server::create_server(int port) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cout << "Could not create socket: " << strerror(errno) << std::endl;
  }

  sockaddr_in addr;

  socklen_t socketaddr_size = sizeof(struct sockaddr);
  memset(&addr, 0, socketaddr_size);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverSocket, (const sockaddr *)&addr, socketaddr_size) == -1) {
    close(serverSocket);
    std::cout << "Could not bind socket: " << strerror(errno) << std::endl;
    return -1;
  }

  if (listen(serverSocket, MAXCONNECTIONS) == -1) {
    std::cout << "Connection listen failed: " << strerror(errno) << std::endl;
    return -1;
  }

  return serverSocket;
}

int Server::accept_connections(int serverSocket) {
  sockaddr_in clientAddr;
  socklen_t addr_size = 0;
  socklen_t sockaddr_size = sizeof(struct sockaddr);
  memset(&clientAddr, 0, sockaddr_size);

  int client_connection =
      accept(serverSocket, (sockaddr *)&clientAddr, &addr_size);

  // Error during connection
  if (client_connection == -1) {
    if (!m_bQuitCommand)
      std::cout << "Connection accept failed: " << strerror(errno) << std::endl;
    return -1;
  }

  // Connection Established
  std::cout << "New Connection: " << inet_ntoa(clientAddr.sin_addr)
            << std::endl;

  return client_connection;
}

int Server::insertClientReader(int clientfd) {
  return dbconn->addClientReader(clientfd);
}

int Server::insertClientWriter(int clientfd) {
  return dbconn->addClientWriter(clientfd);
}

int Server::popClientReader(int clientfd) {
  return dbconn->removeClientReader(clientfd);
}

int Server::popClientWriter(int clientfd) {
  return dbconn->removeClientWriter(clientfd);
}

bool Server::Fork(int clientfd) {
  int pid = fork();

  // Error
  if (pid == -1) {
    std::cout << "Fork failed: " << strerror(errno) << std::endl;
    return -1;
  }

  // Is Child
  if (pid == 0)
    return true;

  // Is parent
  std::pair<int, int> pair = std::make_pair(pid, clientfd);
  procChildren.push_back(pair);
  return false;
}

int Server::receiveMessage(int readerClientfd, char *buf) {
  int readBytes = recv(readerClientfd, &buf, MAXLINE, 0);
  if (readBytes == -1) {
    std::cout << "Client side conenction disconnected: " << strerror(errno)
              << std::endl;
    return -1;
  }

  return readBytes;
}

void Server::spreadMessage(char *message, int bytesOfMessage) {
  int bytesSent = 0;
  sql::ResultSet *res = dbconn->getClientReaders();

  if (!res) {
    delete res;
    std::cout << "Unable to acceess database." << std::endl;
    return;
  }

  while (res->next()) {
    bytesSent = send(res->getInt("_message"), message, bytesOfMessage, 0);
    if (bytesSent == -1) {
      std::cout << "Send failed for Writterfd -> " << res->getInt("_message")
                << ": " << strerror(errno) << std::endl;
    }
  }
}
