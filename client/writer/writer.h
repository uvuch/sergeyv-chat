#ifndef WRITER_H
#define WRITER_H
#include "../client.h"

class Writer : private Client {
private:
  Writer(){};
  ~Writer(){};

  static Writer *m_pInstance;
  static volatile bool m_bRunning;
  static int serverfd;

  int setSigs();

  static void stop(int sig);

public:
  static Writer *instance();
  int connect(const char *pIp, const char *pPort, Client::Type connectionType);
  int sendMessage(int serverfd, char *buf);
  void shutdown();
};

#endif // !READER_H
