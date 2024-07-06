#ifndef SERVER_H
#define SERVER_H

#include <utility>
#include <vector>

class Server {
public:
  static Server *instance();
  static void shutdown();
  static void stop(int);
  void run(int port);

private:
  static Server *m_pInstance;
  static volatile bool m_bQuitCommand;
  int create_server(int port);
  int accept_connections(int serverSocket);

  /** True if is child */
  bool isChild;
  bool Fork(int clientfd);

  int receiveMessage(int clientfd, char *buf);

  //  Proccess ID and Clientfd its servicing
  std::vector<std::pair<int, int>> procChildren;

  void cullWaitingChildren();

  static int serverSocket;

  Server(){};
  ~Server(){};
};

#endif
