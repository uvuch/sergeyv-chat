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

void Server::ServerChild(char *ip, char *port) {
  struct addrinfo *p, *listp, hints;
  char buf[MAXLINE];
  int rc, flags;

  memset(&hints, 0, sizeof(struct addrinfo));

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_socktype = SOCK_STREAM;
  if ((rc = getaddrinfo(ip, NULL, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
    _exit(1);
  }

  flags = NI_NUMERICHOST;
  for (p = listp; p; p = p->ai_next) {
    getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flags);
    printf("%s\n", buf);
  }

  freeaddrinfo(listp);

  exit(0);
}
