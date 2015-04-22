#include "proxy.h"
#include "net.h"
#include "data.h"

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

int tryCache(int sockfd, char *request) {
	char *body = getFromCache(request);

	if (body == NULL)
		return 0;

	writeToClient(sockfd, body);

	free(body);
	return 1;
}

void processRequest(int sockfd, char *message) {
	char *tok = strtok(message, " "); // Filter out the GET message
	tok = strtok(NULL, " "); // tok now has requested site

	char *requested = (char *)malloc(strlen(tok)+1);
	strcpy(requested, tok);

	// Trim the leading '/'
	memmove(requested, requested+1, strlen(requested));
	printf("Processing: %s\n", requested);

	if (!tryCache(sockfd, requested))
		makeHTTPRequest(sockfd, requested);

	free(requested);
}
