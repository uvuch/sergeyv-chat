#include "main.h"
#include "global.h"
#include "server.h"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "3306"
#define MAXLINE 100

bool bRunning = true;

int main(int argc, char **argv) {

  char arg1[20], arg2[6];
  // Check for args and set defaults if none provided
  if (argc == 1)
    std::cout << "Optional settings set " << argv[0] << " <ip> <port>"
              << std::endl;

  (argc == 2 || argc == 3) ? strncpy(arg1, argv[1], sizeof(arg1))
                           : strncpy(arg1, DEFAULT_IP, sizeof(arg1));
  (argc == 3) ? strncpy(arg2, argv[2], sizeof(arg2))
              : strncpy(arg2, DEFAULT_PORT, sizeof(arg2));

  setSigHandlers();

  Server::Instance()->run(arg1, arg2);

  while (bRunning) {
    sleep(1);
  }

  return 0;
}

void server_stop() {
  Server::Instance()->stop();
  free(Server::Instance());
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
