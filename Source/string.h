#ifndef _STRING_H_43674_
#define _STRING_H_43674_

#include <string>
#include <string.h>
#include "defines.h"
using namespace std;

bool isWhitespace(const string& s, const string& ignore=whiteSpace);
string removeStartEndWhitespace(const string& s, const string& whitespace=" \t\n");
char strsub(const char* a, const char* b, int as);
int string_isSubstring(const char* mainStr, const string& subStr);
void strInsert(char* dest, const char* insert, int index);
void stringInsert(char* dest, const string& insert, int index);
void getStructName(const char* line, char* buffer);
bool isEmptyLine(const char* line);
bool structHasTypedef(const char* line);
bool stringContainsChar(const string& s, const char& check);
bool stringContainsChars(const string& s1, const string& s2);
void stringToLower(char * s);
int string_isWord(const char* mainStr, const string& subStr, const string& separators=wordSeparators);

#endif