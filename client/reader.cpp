#include "reader.h"
#include "../sharedNetwork.h"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "54321"
#define CHAR_SIZE_OF_IP 16
#define CHAR_SIZE_OF_PORT 6
#define MAXLINE 100

bool Reader::bRunning = true;
Reader *Reader::m_pInstance = nullptr;

Reader *Reader::Instance() {
  if (m_pInstance == nullptr)
    m_pInstance = new Reader();

  return m_pInstance;
}

int Reader::Connect(char *ServerIp, char *ServerPort) {
  char ip[CHAR_SIZE_OF_IP], port[CHAR_SIZE_OF_PORT];
  if (handle_ip(ServerIp, ip) < 0 || handle_port(ServerPort, port) < 0) {
    bRunning = false;
    return -1;
  }

  int serverfd = client_connection(ip, port);
  if (serverfd < 0)
    return -1;

  // do some sleep and then check with debuger to make sure things are ok
  close_connection(serverfd);

  return 0;
}

int Reader::RecieveMessages(int serverFd) { return 0; }

void Reader::Shutdown() {
  if (m_pInstance != nullptr)
    delete m_pInstance;
}
