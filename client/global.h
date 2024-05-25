#ifndef GLOBAL_H
#define GLOBAL_H

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Stolen from csapp
// ------------------ //
struct rio_t;

ssize_t sio_put(char *msg);

void sio_error(char *msg);

void unix_error(char *msg);

void echo(int connfd);

void rio_readinitb(rio_t *rp, int fd);

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

ssize_t rio_written(int fd, void *userbuf, size_t n);

// ------------------ //

ssize_t sio_write(char *msg);

pid_t Fork();

#endif
