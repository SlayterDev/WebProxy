#include "net.h"
#include "proxy.h"
#include "data.h"

// Keyboard Interrupt handler to properly close socket
void sigtermHandler(int signum) {
	close(sockfd);
	printf("[+] Socket closed\n");
	exit(0);
}

int writeToClient(int sockfd, char *message) {
	if (write(sockfd, message, strlen(message)) < 0) {
		fprintf(stderr, "ERROR writing to socket\n");
		int ret = 1;
		pthread_exit(&ret);
	}

	return 0;
}

void makeHTTPRequest(int sockfd, char *request) {
	int reqSock = socket(AF_INET, SOCK_STREAM, 0);
	int ret = 1;

	if (reqSock < 0) {
		fprintf(stderr, "ERROR making socket for request.\n");
		pthread_exit(&ret);
	}

	struct sockaddr_in req_addr;
	memset((char *)&req_addr, 0, sizeof(req_addr));

	char *requestCopy = (char *)malloc(strlen(request)+1);
	strcpy(requestCopy, request);

	char *host = trimWWW(requestCopy);
	char *tok = strtok(host, "/"); // tok has host.com

	if (checkBlackList(tok)) {
		char response[] = "<html><h1>This site is blocked!</h1></html>";
		printf("Writing response:\n%s\n", response);
		writeToClient(sockfd, response);
		return;
	}

	printf("Connecting to %s\n", tok);

	struct hostent *he;

	if ((he = gethostbyname(tok)) == NULL) {
		fprintf(stderr, "Could not get host\n");
		pthread_exit(&ret);
	}

	memcpy(&req_addr.sin_addr, he->h_addr_list[0], he->h_length);
	req_addr.sin_family = AF_INET;
	req_addr.sin_port = htons(80);

	if (connect(reqSock, (struct sockaddr *)&req_addr, sizeof(req_addr)) < 0) {
		fprintf(stderr, "Thread could not connect to server\n");
		pthread_exit(&ret);
	}

	char *file = strForFile(request);

	char *fullRequest = (char *)malloc(strlen(request)+23);
	sprintf(fullRequest, "GET %s HTTP/1.0\n\n", file);

	printf("=============\nSending:\n%s\n=============\n", fullRequest);

	if (send(reqSock, fullRequest, strlen(fullRequest), 0) < 0) {
		fprintf(stderr, "[-] Failed to send message\n");
		close(reqSock);
		pthread_exit(&ret);
	}

	char serverReply[SERVER_REPLY_SIZE];
	if (recv(reqSock, serverReply, SERVER_REPLY_SIZE, 0) < 0) {
		fprintf(stderr, "[-] Failed to receive message\n");
		close(reqSock);
		pthread_exit(&ret);
	}

	censorSite(serverReply);

	printf("Recieved From Server: %s\n", serverReply);
	writeToClient(sockfd, serverReply);

	writeToCache(request, serverReply);

	close(reqSock);
	free(fullRequest);
}

// Loop started for each thread
void *serverLoop(void *sockfdPtr) {
	printf("Recieved Connection!\n");

	int sockfd = (intptr_t)sockfdPtr;

	int n, ret = 1;
	char buffer[1025];

	memset(buffer, 0, 1024);
	while ((n = read(sockfd, buffer, 1024)) > 0) {
		printf("Recieved: %s\n", buffer);
		processRequest(sockfd, buffer);

		if (n < 0) {
			fprintf(stderr, "ERROR writing to socket\n");
			pthread_exit(&ret);
		}

		memset(buffer, 0, 1024);
	}

	printf("Connection closed\n");

	return NULL;
}

void beginServer(int portnum) {
	int newsockfd, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) {
		fprintf(stderr, "ERROR opening socket\n");
		exit(1);
	}

	memset((char *)&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portnum);

	if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "ERROR binding socket\n");
		exit(1);
	}

	// Setup SIGTERM handler to gracefully close socket
	signal(SIGINT, sigtermHandler);

	printf("Server started...\nUse ^C to quit\n");

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	int clientNo = 0;
	while (1) {
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
		if (newsockfd < 0) {
			fprintf(stderr, "ERROR accepting connection\n");
			exit(1);
		}

		pthread_t thread;

		if (pthread_create(&thread, NULL, serverLoop, (void *)(intptr_t)newsockfd)) {
			fprintf(stderr, "ERROR creating thread\n");
			exit(1);
		}

		clientNo++;		
	}
}
