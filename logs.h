#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "find.h"


void setLogOn(int wk_logs);
void MsgBox(char *message, ...);
void MsgFileError(char *operation, char *path, int nerror);
void printResult(struct pt_param *main_param);

