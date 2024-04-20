#ifndef MAIN_H
#define MAIN_H

#include <csignal>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>

#include "global.h"
#include "server.h"

void setSigHandlers();
void handleExitSignal(int sig);
void handleServerParams(int, char **, char *, char *);

void server_stop();

#endif // !MAIN_H
