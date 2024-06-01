#include <iostream>
#include <signal.h>
#include <unistd.h>
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
  if (signal(SIGINT, Server::stop) == SIG_ERR) {
    std::cout << "Server initialization error (signal handler could not be assigned)!" << std::endl;
    return;
  }

  std::cout << "Starting server listening on a port " << port << std::endl;
  while (!m_bQuitCommand) {
    pause();
  }

  std::cout << "\nServer stopped" << std::endl;
}
