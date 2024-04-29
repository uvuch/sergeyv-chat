#include "main.h"
#include "global.h"
#include "server.h"
#include <cctype>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "54321"
#define CHAR_SIZE_OF_IP 16
#define CHAR_SIZE_OF_PORT 6
#define MAXLINE 100

bool bRunning = true;

int main(int argc, const char **argv) {

  char ip[CHAR_SIZE_OF_IP] = DEFAULT_IP, port[CHAR_SIZE_OF_PORT] = DEFAULT_PORT;

  if (handleServerParams(argc, argv, ip, port) < 0)
    _exit(0);

  std::cout << "Ip: " << ip << "  Port: " << port << std::endl;

  setSigHandlers();

  // Start server
  Server::Instance()->run(port);

  // After server returns
  Server::shutdown();

  return 0;
}

int checkAndCopyPortArg(int argc, const char *argv, char *portArg) {
  bool itsANumber = true;
  for (int i = 0; i < strlen(argv); i++) {
    if (!std::isdigit(argv[i])) {
      itsANumber = false;
      break;
    }

    if (itsANumber)
      std::strncpy(portArg, argv, CHAR_SIZE_OF_PORT - 1);

    else {
      std::cout << "Port should be a valid positive numeric value" << std::endl;
      return -1;
    }
  }

  return 0;
}

int handleServerParams(int argc, const char **argv, char *ipArg,
                       char *portArg) {
  if (argc == 2) {
    if (checkAndCopyPortArg(argc, argv[1], portArg) < 0)
      return -1;
  } else
    for (int i = 1; i < argc; i++) {
      if (i + 1 < argc) {
        // Ip
        if (std::strcmp("-ip", argv[i]) == 0) {
          std::strncpy(ipArg, argv[i + 1], CHAR_SIZE_OF_IP - 1);
          continue;
        }

        // Port
        // Make sure that port is not negative!
        if (std::strcmp("-p", argv[i]) == 0) {
          if (checkAndCopyPortArg(argc, argv[i + 1], portArg) < 0)
            return -1;
        }
      }
    }

  return 0;
}
