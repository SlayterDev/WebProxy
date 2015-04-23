#include "data.h"
#include "proxy.h"

char *getFromCache(char *request) {
	char *filename = (char *)malloc(strlen(request)+9); // 4 for tmp/ 4 for .txt
	sprintf(filename, "tmp/%s.txt", request);

	if (access(filename, F_OK) != -1) {
		FILE *f = fopen(filename, "r");

		if (!f) {
			fprintf(stderr, "Could not open cahe file: %s\n", filename);
			free(filename);
			return NULL;
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
		fprintf(stderr, "Could not write to cache file: %s\n", filename);
		return;
	}

	fprintf(f, "%s\n", content);

	fclose(f);
	free(filename);
}

void initInsults() {
	FILE *f = fopen(INSULTS_FILE, "r");

	if (!f) {
		fprintf(stderr, "Could not open insults file.\n");
		exit(1);
	}

	int i = 0;
	char tmpBuf[22] = "";
	while (fgets(tmpBuf, 22, f) != NULL) {
		tmpBuf[strlen(tmpBuf)-1] = '\0'; // trim newline
		strcpy(insults[i], tmpBuf);
		i++;
	}

	insult_count = i;

	fclose(f);
}

void censorSite(char *content) {
	static char bufCpy[SERVER_REPLY_SIZE] = "";
	strcpy(bufCpy, content);

	char *tok = strtok(bufCpy, " \n\r\t"); // assuming first word is 
											// always '<HTML>'
	while ((tok = strtok(NULL, " \n\r\t")) != NULL) {
		for (int i = 0; i < insult_count; i++) {
			int match = 1;
			for (int j = 0; j < strlen(insults[i]); j++) {
				if (tok[j] != insults[i][j]) {
					match = 0;
					break;
				}
			}

			if (match) {
				// censor the word
				int offset = tok - bufCpy;

				for (int k = offset; k < offset + strlen(tok); k++)
					content[k] = '*';	
			}
		}
	}
}

void initBlackList() {
	FILE *f = fopen(BLACKLIST_FILE, "r");

	if (!f) {
		fprintf(stderr, "No black list found\n");
		return;
	}

	int i = 0;
	char tmpBuf[70] = "";
	while (fgets(tmpBuf, 70, f) != NULL) {
		tmpBuf[strlen(tmpBuf)-1] = '\0';
		strcpy(blackList[i], tmpBuf);
		i++;
	}

	blackList_count = i;

	fclose(f);
}

int checkBlackList(char *request) {
	char *trimmedSite;
	for (int i = 0; i < blackList_count; i++) {
		trimmedSite = trimWWW(blackList[i]);
		if (!strcmp(trimmedSite, request))
			return 1;
	}

	return 0;
}


