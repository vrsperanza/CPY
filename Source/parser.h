#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stack>
using namespace std;

#define LINESZ 16384
#define tabSpaces 4

void generateSource(char * inputFile, char * outputFile, bool beauty = false);
char strsub(char * a, char * b, int as);
int string_isSubstring(char* mainStr, char* subStr);
void getStructName(char * line, char * buffer);
bool structHasTypedef(char * line);
void strInsert(char * dest, char * insert, int index);

#endif