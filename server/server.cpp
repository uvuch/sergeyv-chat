#include "server.h"
#include <arpa/inet.h>
// #include "dbconnector.h"
#include <cerrno>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <vector>

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
  if (serverSocket)
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

  // Get possible vector max size
  int childProcVecSize = calculateVecSize<std::pair<int, int>>(MAXCONNECTIONS);

  key_t key = ftok("shmfile", 0);
  int shmid = shmget(key, childProcVecSize, 0666 | IPC_CREAT);

  // Set the memory to the global variable
  procChildren = (std::vector<std::pair<int, int>> *)shmat(shmid, (void *)0, 0);

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

    cullWaitingChildren();
  }

  if (isChild) {
    char buf[MAXLINE];
    memset(&buf, 0, MAXLINE);

    int bytesSent = 0;

    char sentMessage[] = "test 1 2 3. ";

    while (!m_bQuitCommand) {
      // READER CHECK!
      //
      send(clientfd, sentMessage, strlen(sentMessage), 0);
      //
      //
      //

      bytesSent = receiveMessage(clientfd, (char *)&buf);

      if (bytesSent == -1) {
        m_bQuitCommand = true;
        break;
      }
    }

    // Connection over
  }

  close(serverSocket);

  if (!isChild) {
    // Send sigint to children
    kill(0, SIGINT);

    cullWaitingChildren();
  }

  // You can just delete proChildren shared memory because the std::make_pair's
  // will automaticaly delete themselves
  shmctl(shmid, IPC_RMID, NULL);

  std::cout << getuid() << " exiting." << std::endl;
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
  procChildren->push_back(pair);
  return false;
}

int Server::receiveMessage(int readerClientfd, char *buf) {
  int readBytes = recv(readerClientfd, &buf, MAXLINE, 0);
  if (readBytes == -1) {
    std::cout << "Client side connenction disconnected: " << strerror(errno)
              << std::endl;
    return -1;
  }

  return readBytes;
}

void Server::cullWaitingChildren() {
  int status, pid = 0;
  while ((pid = wait(&status)) > 0)
    if (WIFEXITED(status))
      std::cout << "Culled procChild -> " << pid << std::endl;
}

template <typename T> int Server::calculateVecSize(size_t size) {
  return (int)sizeof(std::vector<T>) + (size * sizeof(T));
}
