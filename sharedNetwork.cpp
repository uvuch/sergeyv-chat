#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "54321"
#define CHAR_SIZE_OF_IP 16
#define CHAR_SIZE_OF_PORT 6
#define MAXLINE 100

#include "sharedNetwork.h"
#include <iostream>
#include <regex>

int client_listen() {
  // Random stuff right now
  int listenfd = 0;

  return listenfd;
}

int handle_ip(char *ip, char *copyTo) {
  try {
    const std::regex rIp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");

    // False
    if (!std::regex_search(ip, rIp)) {
      std::cout << "IP is incorrect! Format: 255.255.255.255 " << std::endl;
      return -1;
    }

    // True
    std::strncpy(copyTo, ip, CHAR_SIZE_OF_IP);
  } catch (const std::regex_error &e) {
    std::cout << "Regex error: " << e.what() << std::endl;
  }

  return 0;
}

int handle_port(char *port, char *copyTo) {
  try {
    const std::regex rPort("^\\d{1,5}$");

    // False
    if (!std::regex_match(port, rPort)) {
      std::cout << "Port is incorrect! Format : 54321 [1 - 5 characters]"
                << std::endl;

      return -1;
    }

    // True
    std::strncpy(copyTo, port, CHAR_SIZE_OF_PORT);

  } catch (const std::regex_error &e) {
    std::cout << "Regex error: " << e.what() << std::endl;
  }
  return 0;
}