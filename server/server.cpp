#include "server.h"
#include "global.h"

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

void childKillTestSigHandler(int sig) {
  sio_write((char *)(std::to_string(getpid()).c_str()));
  sio_write((char *)(" was told to die.\n"));
  Server::Instance()->stop();
  _exit(0);
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
      if (signal(SIGUSR1, childKillTestSigHandler) == SIG_ERR)
        unix_error((char *)("signal error"));

      std::cout << getpid() << " is alive" << std::endl;

      pause();
      break;
    }

    PIDS.push_back(pid);
  }

  // now tell them all to DIE :D
  for (int pid : PIDS) {
    kill(pid, SIGUSR1);
  }

  return 0;
}

void Server::stop() {}

void handleSigChild(int sig) {
  int olderrno = errno;
  while (waitpid(-1, NULL, 0) > 0)
    sio_write((char *)("Reaped child"));
  if (errno != ECHILD)
    sio_error((char *)("waitpid error"));

  errno = olderrno;
}
