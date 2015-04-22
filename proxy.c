#include "proxy.h"
#include "net.h"

char *strForFile(char *request) {
	int i = 0;
	while (request[i] && request[i] != '/')
		i++;

	if (!request[i])
		return "/";
	else
		return request+i;
}

char *trimWWW(char *request) {
	int cnt = 0, i = 0;
	while (request[i] == 'w' && cnt < 3) {
		cnt++;
		i++;
	}

	if (cnt == 3)
		return request+cnt+1; // plus 1 for the dot
	else
		return request;
}

void processRequest(int sockfd, char *message) {
	char *tok = strtok(message, " "); // Filter out the GET message
	tok = strtok(NULL, " "); // tok now has requested site

	char *requested = (char *)malloc(strlen(tok)+1);
	strcpy(requested, tok);

	// Trim the leading '/'
	memmove(requested, requested+1, strlen(requested));
	printf("Processing: %s\n", requested);

	makeHTTPRequest(sockfd, requested);
}