#ifndef DATA_H
#define DATA_H

#include "globals.h"

#define INSULTS_FILE "insults.txt"
#define BLACKLIST_FILE "blacklist.txt"

// Number of insults counted
int insult_count;

// Insults as an array
char insults[244][22];

int blackList_count;

char blackList[100][70];

/*
 Initializes the insults array
*/
void initInsults();

/*
 Attempts to get a requested site from the cache. Returns null if
 the site is not in the array.

 Takes a char pointer to the requested site.
*/
char *getFromCache(char *request);

/*
 Write the content of a website to the cache.

 Takes a char pointer to the site requested, and a char pointer to
 the content of the website.
*/
void writeToCache(char *request, char *content);

/*
 Filters the profanity out of a website.

 Takes a char pointer to the content of the website.
*/
void censorSite(char *content);

void initBlackList();
int checkBlackList(char *request);

#endif
