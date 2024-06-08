#include "server.h"
#include <arpa/inet.h>
#include <cerrno>
#include <errno.h>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 100
#define MAXCONNECTIONS 100

Server *Server::m_pInstance = nullptr;
volatile bool Server::m_bQuitCommand = false;

Server *Server::instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Server;

  return m_pInstance;
}

void Server::shutdown() {
  if (m_pInstance != nullptr)
    delete m_pInstance;
}

void Server::stop(int sig) { m_bQuitCommand = true; }

void Server::run(int port) {
  if (signal(SIGINT, Server::stop) == SIG_ERR) {
    std::cout
        << "Server initialization error (signal handler could not be assigned)!"
        << std::endl;
    return;
  }

  std::cout << "Starting server listening on a port " << port << std::endl;
  std::cout << "Max connections: " << MAXCONNECTIONS << std::endl;

  // Get a server socket
  int serverSocket = create_server(port);
  if (serverSocket == -1) {
    m_bQuitCommand = true;
    return;
  }

  // Set up for listening
  if (connection_listen(serverSocket) == -1) {
    m_bQuitCommand = true;
  }

  std::map<int, char *> connectedClients;

  // Processing requests
  while (!m_bQuitCommand) {
    connection_listen(serverSocket);
  }

  close(serverSocket);

  connectedClients.clear();

  std::cout << "\nServer stopped" << std::endl;
}
int Server::create_server(int port) {
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    std::cout << "Could not create socket!" << std::endl;
  }

  sockaddr_in addr;

  socklen_t socketaddr_size = sizeof(struct sockaddr);
  memset(&addr, 0, socketaddr_size);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverSocket, (const sockaddr *)&addr, socketaddr_size) < 0) {
    close(serverSocket);
    std::cout << "Could not bind socket: " << strerror(errno) << std::endl;
    return -1;
  }

  return serverSocket;
}

int Server::connection_listen(int serverSocket) {
  if (listen(serverSocket, MAXCONNECTIONS) == -1) {
    std::cout << "Connection listen failed: " << strerror(errno) << std::endl;
    return -1;
  }

  // Setup for listening is a success
  return 0;
}

int Server::accept_connections(int serverSocket) {
  sockaddr_in clientAddr;
  socklen_t addr_size = 0;
  socklen_t sockaddr_size = sizeof(struct sockaddr);
  memset(&clientAddr, 0, sockaddr_size);

  int client_connection =
      accept(serverSocket, (sockaddr *)&clientAddr, &addr_size);

  // No connections
  if (client_connection == 0)
    return 0;

  // Error during connection
  if (client_connection == -1) {
    std::cout << "Connection accept failed: " << strerror(errno) << std::endl;
    return -1;
  }

  // Connection Established
  insertClient(client_connection, inet_ntoa(clientAddr.sin_addr));
  std::cout << "New Connection: " << connectedClients[client_connection]
            << std::endl;

  return client_connection;
}

void Server::insertClient(int clientfd, char *charIp) {
  connectedClients[clientfd] = charIp;
}

void Server::popClient(int clientfd) { connectedClients.erase(clientfd); }
