#ifndef NET_H
#define NET_H

#include "globals.h"

void beginServer(int portNum);
void makeHTTPRequest(int sockfd, char *request);

#endif