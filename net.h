#ifndef NET_H
#define NET_H

#include "globals.h"

// Server socket descriptor
int sockfd;

/*
 Start the web proxy server.

 Takes the port number to run the server on as an integer.
*/
void beginServer(int portNum);

/*
 Create a new HTTP request and send it to the server. Function also
 sends the response to the client and writes the response to the 
 cache.

 Takes the socket descriptor of the client and a char pointer of
 the requested site.
*/
void makeHTTPRequest(int sockfd, char *request);

/*
 Write a message to the client's socket.

 Takes the socket descriptor of the client and a char pointer of
 the requested site.
*/
int writeToClient(int sockfd, char *message);

#endif
