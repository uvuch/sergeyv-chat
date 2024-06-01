#ifndef MAIN_H
#define MAIN_H

/** Singleton Reader Class */
class Reader {
private:
  Reader();
  ~Reader();

  static Reader *m_pInstance;

  static bool bRunning;

public:
public:
  /** Get Instance */
  static Reader *Instance();

  /** Connects and Returns serverFd */
  int Connect(char *ip, char *port);

  /** Recieves and Prints messages */
  int RecieveMessages(int serverFd);

  /** Shutdowns and cleans everything */
  void Shutdown();
};

#endif // !MAIN_H
