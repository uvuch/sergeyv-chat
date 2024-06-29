#ifndef CLIENT_H
#define CLIENT_H

class Client {

public:
  Client(){};
  ~Client(){};

  // Returns clientfd
  int server_connect(const char *pIp, const char *pPort);

  static bool checkIp(const char *pIp);
  static bool checkPort(const char *pPort);

  static bool killCalled;

private:
  static int serverfd;
};

#endif // DEBUG
