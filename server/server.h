#ifndef SERVER_H
#define SERVER_H

#include "global.h"

struct rio_t;

class Server {
private:
  static Server *m_pInstance;
  Server(){};
  ~Server(){};

  int open_listen(char *port);
  void setSigHandlers();
  static void handleExitSignal(int sig);
  int Fork();
  void serverClientConnection(int listenfd);

  void echo(int connfd);
  void rio_readinitb(rio_t *rp, int fd);
  ssize_t rio_written(int fd, void *userbuf, size_t n);

  static bool bRunning;
  int pid = 0;

public:
  static Server *Instance();
  static void shutdown();
  int run(char *port);
};

#endif
