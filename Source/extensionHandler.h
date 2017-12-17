#ifndef _ENTENSIONHANDLER_H_43674_
#define _ENTENSIONHANDLER_H_43674_

#include <string>

void removeExtension(char * s);
void replaceHeaderExtension(char * s);
std::string removeExtension(std::string fileName);
std::string removeCharExt(char * fileName);
void stringToCPY(std::string input, char * fileName);
void stringToCPP(std::string input, char * fileName);
void stringToH(std::string input, char * fileName);
std::string getExtension(std::string input);
std::string cropPath(std::string input);
bool isCodeFile(std::string input);
bool isCppFile(std::string input);

#endif