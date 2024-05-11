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
#include <vector>

#define MAXLINE 1024
#define RIO_BUFFSIZE 8192
#define MAX_CHILD_SIZE 100

// NUll the instance at start
Server *Server::m_pInstance = nullptr;
bool Server::bRunning = true;
int Server::globalListenfd = 0;
bool Server::alreadyExiting = false;
int Server::childrenCount = 0;
int Server::childrenQuit = 0;

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
  std::vector<int> childPIDs;

  while (bRunning && !isChild) { // Not Child
    // if open_listen returned fd fork

    if (childrenCount >= MAX_CHILD_SIZE)
      continue;
    if ((globalListenfd = listenfd = open_listen(port)) <= 0)
      continue;
    // Else do everything below

    // At this point you are  confirmed to become a child
    int pid = 0;
    childrenCount++;
    if ((pid = fork()) == 0) {
      // Set group id
      setgid(gid);
      isChild = true;
    } else {
      childPIDs.push_back(pid);
    }
  }

  // Broke out of main loop, do child function now
  if (isChild)
    serverClientConnection(listenfd);
  // Server operand only

  if (!isChild) {
    // Server is going down, get rid of children
    reapChildren(childPIDs);

    // While we wait for children to go, just sit around
    // Once child return check if all children return, exit if true
    while (childPIDs.size() > childrenQuit && childrenCount) {
      pause();
    }
  }

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

  // Listenfd is now clientfdq
  listenfd = accept(listenfd, (sockaddr *)&clientaddr, &clientlen);
  if (listenfd == -1)
    return listenfd;

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

// Send quit signal to children
void Server::reapChildren(std::vector<int> childPIDs) {
  for (int pid : childPIDs) {
    kill(pid, SIGQUIT);
  }
}

void Server::shutdown() {
  if (m_pInstance != nullptr) {
    delete m_pInstance;
  }
}

void Server::setSigHandlers() {
  if (signal(SIGINT, Server::handleExitSignal) == SIG_ERR)
    unix_error((char *)("signal error"));
  if (signal(SIGQUIT, Server::handleQuitSignal) == SIG_ERR)
    unix_error((char *)("signal error"));
  if (signal(SIGCHLD, Server::handleChildSignal) == SIG_ERR)
    unix_error((char *)("signal error"));
}

void Server::handleExitSignal(int sig) {
  bRunning = false;

  // Shutdown all incoming and outgoing connections with the server parent
  if (globalListenfd)
    ::shutdown(globalListenfd, SHUT_RDWR);

  // For when you need to kill a stalled kill process
  if (alreadyExiting) {
    // KILL THIS AND CHILDREN NOW!
    kill(0, SIGKILL);
  }

  alreadyExiting = true;
}

void Server::handleChildSignal(int sig) {
  int status, pid;
  while ((pid = wait(&status)) > 0) {
    childrenQuit++;
    childrenCount--;

    if (WIFEXITED(status))
      sio_write((char *)("Child culled"));
  }
}

void Server::handleQuitSignal(int sig) { bRunning = false; }
