#include "main.h"
#include "reader.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cout << argv[0] << "Ip Port" << std::endl;
    return -1;
  }

  Reader *reader = Reader::Instance();
  int serverfd = reader->Connect(argv[1], argv[2]);

  if (serverfd < 0) {
    reader->Shutdown();
    return -1;
  }

  // Whether the server shuts down or CTRL-C is used, return does not matter
  reader->RecieveMessages(serverfd);

  reader->Shutdown();

  return 0;
}
