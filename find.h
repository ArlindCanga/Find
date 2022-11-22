#ifndef __FIND_H
#define __FIND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include "struct_param.h"
#include "logs.h"
#include "KMP.h"

#define MAX_OCCURRENCES_FOR_WORDS 100
#define MAX_WORDS 100
#define PATH_SIZE 1024
#define LINE_SIZE 4095
#define WORD_SIZE 255

//Questa struct verrà popolata con le parole da trovare
struct find_word
{ 
    char *word; //parola da ricercare
    int tot_occurrences; //occorrenze totali
    struct in_path *in_path[MAX_WORDS]; // Parole trovate nei vari file
};
struct find_word *find_word[MAX_WORDS];
//Questa struct verrà popolata con i path nei quali troverò le words
struct in_path
{
    char *path;
    unsigned int occurrences;
    int line[MAX_OCCURRENCES_FOR_WORDS];
    int position[MAX_OCCURRENCES_FOR_WORDS];
};
char *AbsoluteP(char *path);
void StartFind(struct pt_param *main_param);
void CheckParam(struct pt_param *main_param);
void takeWords(char *wordspath);
void ScanPaths(char *input_path);
void scan_dir(char *path, char *flag);
int Exclude(char *path);
void sortByOcc();
void printResult(struct pt_param *main_param);
void FromReport(char *reportFile, char *reportWord, int times);
void FromFile(char *reportFile, char *reportWord, int times, char* otherFile);
#endif

