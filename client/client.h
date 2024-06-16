#ifndef CLIENT_H
#define CLIENT_H
class Client {

public:
  Client();
  ~Client();

  // Returns clientfd
  int server_connect(const char *pIp, const char *pPort);
  int getIp(const char *pIp);
  int getPort(const char *pPort);

private:
  int portNumberValid(const char *pPortStr);
};

#endif // DEBUG
