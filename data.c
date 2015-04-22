#include "data.h"

char *getFromCache(char *request) {
	char *filename = (char *)malloc(strlen(request)+9); // 4 for tmp/ 4 for .txt
	sprintf(filename, "tmp/%s.txt", request);

	if (access(filename, F_OK) != -1) {
		FILE *f = fopen(filename, "r");

		if (!f) {
			fprintf(stderr, "Could not open cahe file: %s\n", filename);
			exit(1);
		}

		char *fileBuffer = (char *)malloc(SERVER_REPLY_SIZE+1);
		fileBuffer[0] = '\0'; // initialize string
		char tmpBuff[256];

		while (fgets(tmpBuff, 256, f) != NULL) {
			strcat(fileBuffer, tmpBuff);
		}

		fclose(f);
		free(filename);
		return fileBuffer;
	} else {
		free(filename);
		return NULL;
	}
}

void writeToCache(char *request, char *content) {
	char *filename = (char *)malloc(strlen(request)+9);
	sprintf(filename, "tmp/%s.txt", request);

	FILE *f = fopen(filename, "w");

	if (!f) {
		fprintf(stderr, "Could not open cache file: %s\n", filename);
		exit(1);
	}

	fprintf(f, "%s\n", content);

	fclose(f);
	free(filename);
}