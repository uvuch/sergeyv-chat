#include "server.h"
#include <iostream>
#include <stdlib.h>

#define DEFAULT_PORT 54321

/****************************************************************************************************
  Reads command line arguments.
  Returns a default port number if no arguments provided.
  Otherwise it returns an integer port number or -1 if the argument can not be
  translated as a number.
*/
int getPort(int, const char **);

/****************************************************************************************************
  Validates the string can be used as a port number
  Returns true if the string consists of digits only.
*/
bool portNumberValid(const char *);

/****************************************************************************************************/
int main(int argc, const char **argv) {
  int port = getPort(argc, argv);
  if (port < 0) {
    std::cout << "Error: invalid port number" << std::endl;
    return -1;
  }

  Server::instance()->run(port);

  Server::shutdown();
  return 0;
}

bool portNumberValid(const char *pPortStr) {
  for (int i = 0; i < strlen(pPortStr); ++i)
    if (pPortStr[i] < '0' || pPortStr[i] > '9')
      return false;

  return true;
}

int getPort(int argc, const char **argv) {
  if (argc < 2)
    return DEFAULT_PORT;

  const char *pPortArg = argv[1];
  if (!portNumberValid(pPortArg))
    return -1;

  return atoi(pPortArg);
}
