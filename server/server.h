#ifndef SERVER_H
#define SERVER_H

#include <map>
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
  int connection_listen(int serverSocket);
  int accept_connections(int serverSocket);

  std::map<int, char *> connectedClients;
  void insertClient(int clientfd, char *charIp);
  void popClient(int clientfd);

  Server(){};
  ~Server(){};
};

#endif
