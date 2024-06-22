#ifndef CLIENT_H
#define CLIENT_H

class Client {

public:
  Client(){};
  ~Client(){};

  // Returns clientfd
  int server_connect(const char *pIp, const char *pPort);

  int checkIp(const char *pIp);
  int checkPort(const char *pPort);

  static bool killCalled;

private:
  static int serverfd;
};

#endif // DEBUG
