#include "global.h"
// Mostly Stolen from csapp
// ------------------ //

#define MAXLINE 1024
#define RIO_BUFSIZE 8192

// Struct for buffered read and writes
struct rio_t {
  int rio_fd;
  int rio_cnt;
  char *rio_bufptr;
  char rio_buf[RIO_BUFSIZE];
};

ssize_t sio_put(char *msg) { return write(STDOUT_FILENO, msg, strlen(msg)); }

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

void echo(int connfd) {
  size_t n;
  char buf[MAXLINE];
  rio_t rio;
  rio_readinitb(&rio, connfd);

  while ((n = rio_readlineb(&rio, buf, MAXLINE)) > 0) {
    printf("%d: recieved %d bytes\n", getpid(), (int)n);
    rio_written(connfd, buf, n);
  }
}

ssize_t rio_read(rio_t *rp, char *userbuf, size_t n) {
  int cnt;

  while (rp->rio_cnt <= 0) {
    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
    if (errno != EINTR) // Interupted by sig handler return
      return -1;
    else if (rp->rio_cnt == 0) // EOF
      return 0;
    else
      rp->rio_bufptr = rp->rio_buf; // Reset pointer
  }

  // Copy min(n, rp->rio_cnt) bytes from internal buf to ouser buf
  cnt = n;
  if (rp->rio_cnt < n)
    cnt = rp->rio_cnt;
  memcpy(userbuf, rp->rio_bufptr, cnt);
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;
  return cnt;
}

ssize_t rio_written(int fd, void *userbuf, size_t n) {
  size_t nleft = n;
  ssize_t nwritten;
  char *bufp = (char *)userbuf;

  while (nleft > 0) {
    if ((nwritten = write(fd, bufp, nleft)) <= 0) {
      if (errno == EINTR) // Interrupted by sig handler return
        nwritten = 0;
      else
        return -1;
    }
    nleft -= nwritten;
    bufp += nwritten;
  }

  return n;
}

void rio_readinitb(rio_t *rp, int fd) {
  rp->rio_fd = fd;
  rp->rio_cnt = 0;
  rp->rio_bufptr = rp->rio_buf;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
  int n, rc;
  char c, *bufp = (char *)usrbuf;

  for (n = 1; n < maxlen; n++) {
    if ((rc = rio_read(rp, &c, 1)) == 1) {
      *bufp++ = c;
      if (c == '\n') {
        n++;
        break;
      }
    } else if (rc == 0) {
      if (n == 1)
        return 0;
      else
        break;
    } else
      return -1;
  }
  *bufp = 0;
  return n - 1;
}

// ------------------ //

ssize_t sio_write(char *msg) { return sio_put(msg); }

pid_t Fork() {
  pid_t pid;

  if ((pid = fork()) < 0)
    unix_error((char *)("fork error"));

  return pid;
}
