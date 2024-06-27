#include <iostream>
#include <signal.h>
#include <unistd.h>
 #include <sys/socket.h>
#include "server.h"

Server *Server::m_pInstance = nullptr;
volatile bool Server::m_bQuitCommand = false;

Server *Server::instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Server;

  return m_pInstance;
}

void Server::shutdown() {
  if (m_pInstance != nullptr) delete m_pInstance;
}

void Server::stop(int sig) {
  m_bQuitCommand = true;
}

void Server::run(int port) {
  this->port = port;

  if (signal(SIGINT, Server::stop) == SIG_ERR) {
    std::cout << "Server initialization error (signal handler could not be assigned)!" << std::endl;
    return;
  }

  int listeningSocket = prepareListeningSocket();
  if (listeningSocket < 0) return;

  pid_t childProcess;
  struct sockaddr clientAddr;
  socklen_t slientAddrLength;

  while (!m_bQuitCommand) {
    int clientSocket = accept(listeningSocket, &clientAddr, &slientAddrLength);
    if (clientSocket < 0) {
      perror(NULL);
      std::cout << "Could not accept incoming connection request." << std::endl;
      continue;
    }

    childProcess = fork();
    if (childProcess) { // parent process flow
      // todo: remember child process id in the PID-collection
      close(clientSocket);
    }
    else { // child process flow
      std::cout << "Client connected to the server." << std::endl;
      close(listeningSocket); // child process doesn't need the listening socket
      
      sleep(5); // read/write loop (client/server conversation happens here)
      close(clientSocket);
      std::cout << "Client disconnected from the server." << std::endl;
      return;
    }
  }

  std::cout << "\nServer stopped" << std::endl;
}

int Server::prepareListeningSocket() {
  int listeningSocket = socket(PF_INET, SOCK_STREAM, 0);
  // 2. bind
  // 3. listen

  if (listeningSocket < 0) {
    std::cout << "Unable to start the server" << std::endl;
  }
  else std::cout << "Starting server listening on a port " << this->port << std::endl;

  return listeningSocket;
}
