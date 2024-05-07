#include "server.h"
#include "global.h"
#include "main.h"

#include <csignal>
#include <cstdio>
#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 1024
#define RIO_BUFFSIZE 8192

// NUll the instance at start
Server *Server::m_pInstance = nullptr;
bool Server::bRunning = true;

Server *Server::Instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Server;

  return m_pInstance;
}

int Server::run(char *port) {
  std::cout << "Server started  PID: " << getpid() << std::endl;

  int listenfd = 0;
  bool isChild = false;

  gid_t gid = getgid();

  while (bRunning && !isChild) { // Not Child
    if ((listenfd = open_listen(port)) > 0)
      continue; // If nothing found
    else
      break;

    // is Child
    int pid = 0;
    if ((pid = fork()) == 0) {
      // Set group id
      setgid(gid);
      isChild = true;
    }
  }

  // Broke out of main loop, do child function now
  if (bRunning && isChild)
    serverClientConnection(listenfd);

  std::cout << "Server stopped" << std::endl;
  return 0;
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

  socklen_t clientlen = sizeof(struct sockaddr_storage);
  struct sockaddr_storage clientaddr;
  char client_hostname[MAXLINE], client_port[MAXLINE];

  // Listenfd is now clientfd
  listenfd = accept(listenfd, (sockaddr *)&clientaddr, &clientlen);
  getnameinfo((sockaddr *)&clientaddr, clientlen, client_hostname, MAXLINE,
              client_port, MAXLINE, 0);
  printf("%d: Connected to (%s, %s)\n", getpid(), client_hostname, client_port);

  return listenfd;
}

void Server::serverClientConnection(int connfd) {
  while (Server::bRunning) {
    ::echo(connfd);
    close(connfd);
  }

  // If exits the connection is over
}

void Server::shutdown() {
  if (m_pInstance != nullptr) {
    delete m_pInstance;
  }
}

void Server::setSigHandlers() {
  if (signal(SIGINT, Server::handleExitSignal) == SIG_ERR)
    unix_error((char *)("signal error"));
}

void Server::handleExitSignal(int sig) { bRunning = false; }
