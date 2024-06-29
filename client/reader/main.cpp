#include "reader.h"
#include <iostream>

#define MAXSIZE 100

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << argv[0] << " ip port" << std::endl;
    return 1;
  }

  int serverfd = 0;
  serverfd = Reader::instance()->connect(argv[1], argv[2]);
  if (serverfd <= 0) {
    Reader::instance()->shutdown();
    return 0;
  }

  std::cout << "Reader started." << std::endl;

  char buf[MAXSIZE];

  Reader::instance()->receiveMessages(serverfd, (char *)&buf);

  Reader::instance()->shutdown();

  std::cout << "Exiting Reader." << std::endl;

  return 0;
}
