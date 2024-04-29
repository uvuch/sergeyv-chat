#include "server.h"
#include "global.h"

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

int Server::run(char *ip, char *port) {

  // if (signal(SIGINT, this->stop) == SIG_ERR)
  //   unix_error((char *)("signal error"));

  // test
  std::vector<pid_t> PIDS;
  pid_t group_pid = getpid();

  setgid(group_pid);

  for (int i = 0; i < 10; i++) {
    pid_t pid;

    // make children
    if ((pid = Fork()) == 0) {
      setgid(group_pid);
      // child stuff
    }

    PIDS.push_back(pid);
  }

  return 0;
}

void Server::stop() {}

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
