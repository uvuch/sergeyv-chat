#include "global.h"
// Next 3 stolen from csapp
// ------------------ //
ssize_t sio_put(char *msg) { return write(STDOUT_FILENO, msg, sizeof(msg)); }

void sio_error(char *msg) {
  sio_put(msg);
  _exit(1);
}

void unix_error(char *msg) {
  sio_put(msg);
  sio_put((char *)(": "));
  sio_error(strerror(errno));
  _exit(0);
}
// ------------------ //

ssize_t sio_write(char *msg) { return sio_put(msg); }

pid_t Fork() {
  pid_t pid;

  if ((pid = fork()) < 0)
    unix_error((char *)("fork error"));

  return pid;
}
