#include "server.h"
#include "global.h"
#include "main.h"

#include <csignal>
#include <cstdio>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#define MAXLINE 1024

// NUll the instance at start
Server *Server::m_pInstance = nullptr;

Server *Server::Instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Server;

  return m_pInstance;
}

int Server::open_listen(char *port) {
  struct addrinfo hints, *listp, *p;
  int listenfd, optval = 1;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
  hints.ai_flags |= AI_NUMERICSERV;
  if (int err = getaddrinfo(NULL, port, &hints, &listp) != 0) {
    std::cout << "Getaddrinfo error: " << gai_strerror(err) << std::endl;
  }

  for (p = listp; p; p = p->ai_next) {
    if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
      continue; // Failed

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
               sizeof(int));

    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
      break; // Success

    close(listenfd); // Bind failed
  }

  freeaddrinfo(listp);
  if (!p) // No addresses worked
    return -1;

  if (listen(listenfd, MAXLINE) < 0) {
    close(listenfd);
    return -1;
  }

  return listenfd;
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
