#include "writer.h"
#include <arpa/inet.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#define MAXLINE 100

Writer *Writer::m_pInstance = nullptr;
volatile bool Writer::m_bRunning = false;
int Writer::serverfd = 0;

Writer *Writer::instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Writer;

  return m_pInstance;
}

int Writer::connect(const char *pIp, const char *pPort,
                    Client::Type connectionType) {
  int serverfd = 0;

  serverfd = server_connect(pIp, pPort, connectionType);
  if (serverfd < 0)
    return -1;

  Writer::serverfd = serverfd;
  m_bRunning = true;

  return serverfd;
}

int Writer::sendMessage(int serverfd, char *buf) {
  int readBytes = 0;

  while (m_bRunning && !Client::killCalled) {
    strcpy(buf, "Hey");
    readBytes = send(serverfd, buf, MAXLINE, 0);

    if (readBytes < 0) {
      // If SIGINT is responsible for the error
      if (Client::killCalled)
        std::cout << "Send failed: " << strerror(errno) << std::endl;

      m_bRunning = false;
      return -1;
    }
  }

  return 0;
}

void Writer::shutdown() {
  if (m_pInstance)
    delete m_pInstance;
}

int Writer::setSigs() {
  if (signal(SIGINT, stop) == SIG_ERR) {
    std::cout << "Signal failed: " << strerror(errno) << std::endl;
    return -1;
  }
  return 0;
}

void Writer::stop(int sig) {
  Client::killCalled = true;
  Writer::m_bRunning = false;

  if (serverfd > 0) {
    close(serverfd);
  }
}
