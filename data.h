#ifndef DATA_H
#define DATA_H

#include "globals.h"

#define INSULTS_FILE "insults.txt"

int insult_count;
char insults[244][22];

void initInsults();
char *getFromCache(char *request);
void writeToCache(char *request, char *content);
void censorSite(char *content);

#endif
