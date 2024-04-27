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
int handleServerParams(int, const char **, char *, char *);

#endif // !MAIN_H
