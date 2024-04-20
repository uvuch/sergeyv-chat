#include "main.h"
#include "global.h"
#include "server.h"
#include <cstring>
#include <unistd.h>

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "54321"
#define CHAR_SIZE_OF_IP 16
#define CHAR_SIZE_OF_PORT 6
#define MAXLINE 100

bool bRunning = true;

int main(int argc, char **argv) {

  char arg1[CHAR_SIZE_OF_IP], arg2[CHAR_SIZE_OF_PORT];

  handleServerParams(argc, argv, arg1, arg2);
  std::cout << "Ip: " << arg1 << "  Port: " << arg2 << std::endl;

  setSigHandlers();

  Server::Instance()->run(arg1, arg2);

  while (bRunning) {
    sleep(1);
  }

  return 0;
}

void handleServerParams(int argc, char **argv, char *ipArg, char *portArg) {
  bool ipWritten, portWritten = false;
  if (argc > 1)
    for (int i = 1; i < argc; i++) {
      if (i + 1 < argc) {
        // Ip
        if (std::strcmp(static_cast<const char *>("ip"), argv[i])) {
          std::strncpy(ipArg, argv[i + 1], CHAR_SIZE_OF_IP - 1);
          ipWritten = true;
          continue;
        }

        // Port
        else if (std::strcmp(static_cast<const char *>("-p"), argv[i])) {
          // Make sure that port is not negative!
          if (argv[i + 1][0] != '-') {
            std::strncpy(portArg, argv[i + 1], CHAR_SIZE_OF_PORT - 1);
            portWritten = true;
            continue;
          }

          else {
            std::cout << "Port cant be negative" << std::endl;
            server_stop();
          }
        }
      }
    }

  if (!ipWritten)
    std::strncpy(ipArg, DEFAULT_IP, sizeof(DEFAULT_IP));
  if (!portWritten)
    std::strncpy(portArg, DEFAULT_PORT, sizeof(DEFAULT_PORT));
}

void server_stop() {
  Server::Instance()->stop();
  free(Server::Instance());
  exit(0);
}

void setSigHandlers() {
  if (signal(SIGINT, handleExitSignal) == SIG_ERR)
    unix_error((char *)("signal error"));
}

void handleExitSignal(int sig) {
  if (bRunning)
    server_stop();
  bRunning = false;
  sio_write((char *)("Got SIGINT\n"));
  // _exit(0);
}
