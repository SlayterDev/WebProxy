#ifndef PROXY_H
#define PROXY_H

#include "globals.h"

/*
 Return the requested file without the domain name.

 Takes a char pointer to the requested site and file.
*/
char *strForFile(char *request);

/*
 Processes the request of the client.

 Takes the socket descriptor of the client and a char pointer to
 the requested site.
*/
void processRequest(int sockfd, char *message);

/*
 Strips the 'www' from a request. (e.g. www.example.com => example.com)

 Takes a char pointer to the requested site. 
*/
char *trimWWW(char *request);

#endif
