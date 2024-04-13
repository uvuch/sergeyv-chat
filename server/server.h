#ifndef SERVER_H
#define SERVER_H

#include "global.h"

class Server {
private:
  static Server *m_pInstance;
  Server(){};
  ~Server(){};

public:
  static Server *Instance();
  int run(char *ip, char *port);
  void stop();
};

#endif
