#include "../client.h"
#include "writer.h"
#include <iostream>

#define MAXSIZE 100

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << argv[0] << " ip port" << std::endl;
    return 1;
  }

  // Valid check
  if (!Client::checkIp(argv[1]) || !Client::checkPort(argv[2]))
    return 1;

  int serverfd = 0;
  serverfd =
      Writer::instance()->connect(argv[1], argv[2], Client::Type::Writer);
  if (serverfd <= 0) {
    Writer::instance()->shutdown();
    return 0;
  }

  std::cout << "Writer started." << std::endl;

  char buf[MAXSIZE];

  while (Writer::instance()->sendMessage(serverfd, (char *)&buf) >= 0)
    ;

  Writer::instance()->shutdown();

  std::cout << "Exiting Writer." << std::endl;

  return 0;
}
