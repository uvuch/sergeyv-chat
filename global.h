enum Type { Reader = 0, Writer = 1 };

class Errors {
public:
  enum Connection {
    couldNotCreateSocket = -1,
    couldNotBindSocket = -2,
    connectionListenFailed = -3,
    clientAcceptFailed = -4,
    clientSideDisconnected = -5
  };
  enum Fork { forkFailed = -1 };
};
