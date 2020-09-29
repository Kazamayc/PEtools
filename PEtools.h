#ifndef _PETOOLS_h_
#define _PETOOLS_h_
#include <stdio.h>
#include <memory>
int getlen(FILE* fp);
char* FileGet(char* PATH);
void PEHeadGet(char* PATH);
#endif