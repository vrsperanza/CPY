#ifndef _LINE_H_43674_
#define _LINE_H_43674_

#include <string>
#include <vector>
#include "defines.h"

using namespace std;


int stringClosePosition(char * line, int startPos);
int closePosition(char * line, int startPos, char open, char close);
void addPahrenthesis(char * s);
bool isExternalInclude(char * line);
int internalInclude(char * line);
string rawInclude(char * line);
vector<string> splitWords(char * line, string firstWordRequired="", string separators=wordSeparators);
string getFirstWord(char * line, string separators=wordSeparators);
vector<string> smartSplitWords(char * line, string firstWordRequired="", string separators=wordSeparators, bool keepSplits=false);

#endif