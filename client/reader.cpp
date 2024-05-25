#include <cctype>
#include <cstring>
#include <iostream>
#include <regex.h>
#include <regex>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "54321"
#define CHAR_SIZE_OF_IP 16
#define CHAR_SIZE_OF_PORT 6
#define MAXLINE 100

bool bRunning = true;

int main(int argc, char *argv[]) {
  char ip[CHAR_SIZE_OF_IP], port[CHAR_SIZE_OF_PORT];

  if (argc != 3) {
    std::cout << "./" << argv[0] << "<ip> <port>" << std::endl;
    return -1;
  } else {
    const std::regex rIp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
    const std::regex rPort("^\\d{1,5}$");
    std::smatch matches;

    if (std::regex_match(argv[1], rIp, std::regex_constants::match_any))
      strncpy(ip, argv[1], CHAR_SIZE_OF_IP);
    else {
      std::cout << "Ip not valid!" << std::endl;
      return -1;
    }
    if (std::regex_match(argv[2], rPort, std::regex_constants::match_any))
      strncpy(port, argv[2], CHAR_SIZE_OF_PORT);
    else {
      std::cout << "Port not valid!" << std::endl;
      return -1;
    }
  }

  std::cout << "IP: " << ip << ";   Port: " << port << ";" << std::endl;

  // connectToServer(ip, port);

  return 0;
}

// make ip match function, and port match function
