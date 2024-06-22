#include "reader.h"
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 100

Reader *Reader::m_pInstance = nullptr;
volatile bool Reader::m_bRunning = false;
int Reader::serverfd = 0;

Reader *Reader::instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Reader;

  return m_pInstance;
}

int Reader::connect(const char *pIp, const char *pPort) {
  if (checkIp(pIp) == -1 || checkPort(pPort) == -1)
    return -1;

  int serverfd = 0;

  serverfd = server_connect(pIp, pPort);
  if (serverfd < 0)
    return -1;

  Reader::serverfd = serverfd;
  m_bRunning = true;

  return serverfd;
}

int Reader::receiveMessages(int serverfd, char *buf) {
  int readBytes = 0;
  std::time_t currentTime;

  while (m_bRunning && !Client::killCalled) {
    readBytes = recv(serverfd, &buf, MAXLINE, 0);

    if (readBytes < 0) {
      // If SIGINT is responsible for the error
      if (Client::killCalled)
        std::cout << "Recv failed: " << strerror(errno) << std::endl;

      m_bRunning = false;
      return -1;
    }

    currentTime = std::time(nullptr);

    std::cout << std::asctime(std::localtime(&currentTime)) << ": " << buf
              << std::endl;
  }

  return 0;
}

void Reader::shutdown() {
  if (m_pInstance)
    delete m_pInstance;
}

int Reader::setSigs() {
  if (signal(SIGINT, stop) == SIG_ERR) {
    std::cout << "Signal failed: " << strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

void Reader::stop(int sig) {
  Client::killCalled = true;
  Reader::m_bRunning = false;

  if (serverfd > 0) {
    close(serverfd);
  }
}
