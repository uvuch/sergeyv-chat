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
#define RIO_BUFFSIZE 8192

struct {
  int rio_fd;
  int rio_cnt;
  char *rio_bufptr;
  char *rio_buf[RIO_BUFFSIZE];
} rio_t;

// NUll the instance at start
Server *Server::m_pInstance = nullptr;

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

  while (bRunning) {
    if ((listenfd = open_listen(port)) < 0)
      continue; // If nothing found

    // is Child
    if ((pid = fork()) == 0) {
      // Set group id
      setgid(gid);
      isChild = true;
      break;
    }

    // Broke out of main loop, do child function now
    if (isChild)
      serverClientConnection(listenfd);
  }

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

  return listenfd;
}

void Server::serverClientConnection(int listenfd) {
  socklen_t clientlen = sizeof(struct sockaddr_storage);
  int connfd = 0;
  struct sockaddr_storage clientaddr;
  char client_hostname[MAXLINE], client_port[MAXLINE];

  while (1) {
    connfd = accept(listenfd, (sockaddr *)&clientaddr, &clientlen);
    getnameinfo((sockaddr *)&clientaddr, clientlen, client_hostname, MAXLINE,
                client_port, MAXLINE, 0);
    printf("%d: Connected to (%s, %s)\n", pid, client_hostname, client_port);
    echo(connfd);
    close(connfd);
  }

  // If exits the connection is over
}

void Server::echo(int connfd) {
  size_t n;
  char buf[MAXLINE];
  struct rio_t rio;
  rio_readinitb(&rio, connfd);

  while ((n = rio_readinitb(&rio, buf, MAXLINE)) != 0) {
    printf("%d: recieved %d bytes\n", pid, (int)n);
    rio_written(connfd, buf, n);
  }
}

void Server::rio_readinitb(rio_t *rp, int fd) {
  rp->rio_fd = fd;
  rp->rio_cnt = 0;
  rp->rio_bufptr = rp->rio_buf;
}

ssize_t Server::rio_written(int fd, void *userbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char *bufp = (char *)userbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR) // Interrupted by sig handler return
        nwritten = 0;
      else
        return -1;
    }
    nleft -= nwritten;
    bufp += nwritten;
  }

  return n;
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
