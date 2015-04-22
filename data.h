#ifndef DATA_H
#define DATA_H

#include "globals.h"

char *getFromCache(char *request);
void writeToCache(char *request, char *content);

#endif