#ifndef _HEADER_H_
#define _HEADER_H_

#include <vector>
#include <string>
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>
void generateHeader(char * inputFile, char * outputFile);
vector<string> getDependencies(char * inputFile);
void replaceRawIncludes(char * filename);

#endif