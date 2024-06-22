#ifndef READER_H
#define READER_H
#include "../client.h"

class Reader : private Client {
private:
  Reader(){};
  ~Reader(){};

  static Reader *m_pInstance;
  static volatile bool m_bRunning;
  static int serverfd;

  int setSigs();

  static void stop(int sig);

public:
  static Reader *instance();
  int connect(const char *pIp, const char *pPort);
  int receiveMessages(int serverfd, char *buf);
  void shutdown();
};

#endif // !READER_H
