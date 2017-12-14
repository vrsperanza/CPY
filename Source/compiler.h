#ifndef _COMPILER_H_
#define _COMPILER_H_
bool fileExist (char * filename);
void stringToCPP(string input, char * fileName);
void stringToH(string input, char * fileName);
void stringToCPY(string input, char * fileName);
bool isOverwritable(char * fileName);
void placeAutoTag(char * fileName);
#endif