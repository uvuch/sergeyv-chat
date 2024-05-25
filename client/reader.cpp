#include "../sharedNetwork.h"
#include <iostream>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "54321"
#define CHAR_SIZE_OF_IP 16
#define CHAR_SIZE_OF_PORT 6
#define MAXLINE 100

bool bRunning = true;

int main(int argc, char *argv[]) {
  char ip[CHAR_SIZE_OF_IP], port[CHAR_SIZE_OF_PORT] = {};

  if (argc != 3) {
    std::cout << argv[0] << "<ip> <port>" << std::endl;
    return -1;
  } else {
    if (handle_ip(argv[1], ip) < 0 || handle_port(argv[2], port) < 0) {
      bRunning = false;
      return -1;
    }
  }

  std::cout << "IP: " << ip << ";   Port: " << port << ";" << std::endl;

  // connectToServer(ip, port);

  return 0;
}

// make ip match function, and port match function
