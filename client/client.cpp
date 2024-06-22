#include "client.h"
#include <arpa/inet.h>
#include <csignal>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <regex>
#include <sys/socket.h>
#include <unistd.h>

int Client::serverfd = 0;
bool Client::killCalled = false;

int Client::server_connect(const char *pIp, const char *pPort) {
  int clientSock = socket(PF_INET, SOCK_STREAM, 0);

  if (clientSock < 0) {
    std::cout << "Failed to create socket: " << strerror(errno) << std::endl;
    return -1;
  }

  sockaddr_in *serverAddr;
  socklen_t socklen = sizeof(struct sockaddr);
  memset(serverAddr, 0, socklen);

  serverAddr->sin_family = AF_INET;
  serverAddr->sin_port = htons(atoi(pPort));

  if (inet_pton(AF_INET, pIp, &serverAddr->sin_addr) <= 0) {
    std::cout << "Failed to convert Ip to network: " << strerror(errno)
              << std::endl;
    return -1;
  }

  int err = connect(clientSock, (struct sockaddr *)serverAddr, socklen);
  if (err < 0) {
    std::cout << "Failed to Connect: " << strerror(errno) << std::endl;
    return -1;
  }
  serverfd = clientSock;

  return serverfd;
}

int Client::checkIp(const char *pIp) {
  const std::regex rIp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");

  if (!std::regex_search(pIp, rIp)) {
  std:;
    std::cout << "IP format is incorrect. Format => 255.255.255.255"
              << std::endl;
    return -1;
  }

  return 0;
}

int Client::checkPort(const char *pPort) {
  const std::regex rPort("^\\d{1,5}$");

  if (!std::regex_search(pPort, rPort)) {
    std::cout << "Port format is incorrect. Format => 1..5 Integers"
              << std::endl;
    return -1;
  }

  return 0;
}
