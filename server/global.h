#ifndef GLOBAL_H
#define GLOBAL_H

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// Next 3 stolen from csapp
// ------------------ //
ssize_t sio_put(char *msg);

void sio_error(char *msg);

void unix_error(char *msg);
// ------------------ //

ssize_t sio_write(char *msg);

pid_t Fork();

#endif
