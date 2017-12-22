#ifndef _STRING_H_43674_
#define _STRING_H_43674_

#include <string>
#include "defines.h"
using namespace std;

bool isWhitespace(string s, string ignore=whiteSpace);
string removeTrailingWhitespace(string s, string whitespace=" \t\n");
char strsub(char * a, char * b, int as);
int string_isSubstring(char* mainStr, string subStr);
void strInsert(char * dest, char * insert, int index);
void stringInsert(char * dest, string insert, int index);
void implyFunctionParametersType(char * s);
void getStructName(char * line, char * buffer);
bool isEmptyLine(char * line);
bool structHasTypedef(char * line);
bool stringContainsChar(string s, char check);
void stringToLower(char * s);
int string_isWord(char* mainStr, string subStr, string separators=wordSeparators);

#endif