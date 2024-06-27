#ifndef SERVER_H
#define SERVER_H

class Server {
public:
  static Server *instance();
  static void shutdown();
  static void stop(int);
  void run(int port);

private:
  static Server *m_pInstance;
  static volatile bool m_bQuitCommand;
  int port;

  int prepareListeningSocket();

  Server(){};
  ~Server(){};
};

#endif
