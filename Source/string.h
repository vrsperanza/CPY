#ifndef _STRING_H_43674_
#define _STRING_H_43674_

#include <string>
using namespace std;

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

#endif