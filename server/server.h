#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <vector>
class Server {
public:
  static Server *instance();
  static void shutdown();
  static void stop(int);
  void run(int port);

  static volatile bool m_bQuitCommand;

private:
  static Server *m_pInstance;
  int create_server(int port);
  int accept_connections(int serverSocket);

  /** True if is child */
  bool Fork();
  bool isChild;

  std::vector<int> procChildren;
  std::map<int, char *> connectedClients;
  void insertClient(int clientfd, char *charIp);
  void popClient(int clientfd);

  static int serverSocket;

  Server(){};
  ~Server(){};
};

#endif
