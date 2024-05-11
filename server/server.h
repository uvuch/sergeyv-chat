#ifndef SERVER_H
#define SERVER_H

#include "global.h"
#include <vector>

struct rio_t;

class Server {
private:
  static Server *m_pInstance;
  Server(){};
  ~Server(){};

  int open_listen(char *port);
  void setSigHandlers();
  static void handleExitSignal(int sig);
  static void handleChildSignal(int sig);
  static void handleQuitSignal(int sig);
  void serverClientConnection(int listenfd);
  void reapChildren(std::vector<int> childPIDs);

  static bool bRunning;
  static bool alreadyExiting;
  static int childrenQuit;

public:
  static Server *Instance();
  static void shutdown();
  int run(char *port);
};

#endif
