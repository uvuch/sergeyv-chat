#ifndef SERVER_H
#define SERVER_H

#include "global.h"
#include "main.h"

class Server {
private:
  static Server *m_pInstance;
  Server(){};
  ~Server(){};

  void setSigHandlers();
  static void handleExitSignal(int sig);

  static bool bRunning;

public:
  static Server *Instance();
  static void shutdown();
  int run(char *port);
};

#endif
