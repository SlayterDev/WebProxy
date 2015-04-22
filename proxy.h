#ifndef PROXY_H
#define PROXY_H

#include "globals.h"

void processRequest(int sockfd, char *message);
char *trimWWW(char *request);
char *strForFile(char *request);

#endif