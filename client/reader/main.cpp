#include "reader.h"
#include <iostream>

#define MAXSIZE 100

int main(int argc, char *argv[]) {

  if (argc != 3) {
    std::cout << argv[0] << " ip port" << std::endl;
    return 1;
  }

  Reader *reader = Reader::instance();

  int serverfd = 0;
  serverfd = reader->connect(argv[1], argv[2]);
  if (serverfd <= 0) {
    reader->shutdown();
    return 0;
  }

  std::cout << "Reader started." << std::endl;

  char buf[MAXSIZE];

  reader->receiveMessages(serverfd, (char *)&buf);

  reader->shutdown();

  std::cout << "Exiting Reader." << std::endl;

  return 0;
}
