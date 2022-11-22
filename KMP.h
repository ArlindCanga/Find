#ifndef __KMP_H
#define __KMP_H
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "find.h"
#include "logs.h"

void get_matches(char *path);
int KMPcheck(const char *text, const char *word, int textLen, int wordLen,int nShift, int nLine);
#endif