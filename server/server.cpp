#include "server.h"
#include "global.h"
#include "main.h"

#include <csignal>
#include <cstdio>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// NUll the instance at start
Server *Server::m_pInstance = nullptr;

Server *Server::Instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Server;

  return m_pInstance;
}

void Server::shutdown() {
  if (m_pInstance != nullptr) {
    delete m_pInstance;
  }
}

int Server::run(char *port) {
  std::cout << "Server started  PID: " << getpid() << std::endl;

  while (bRunning)
    sleep(1);

  std::cout << "Server stopped" << std::endl;
  return 0;
}

void Server::setSigHandlers() {
  if (signal(SIGINT, Server::handleExitSignal) == SIG_ERR)
    unix_error((char *)("signal error"));
}

void Server::handleExitSignal(int sig) { bRunning = false; }
