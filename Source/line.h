#ifndef _LINE_H_43674_
#define _LINE_H_43674_

#include <string>

void addPahrenthesis(char * s);
bool isExternalInclude(char * line);
int internalInclude(char * line);
std::string rawInclude(char * line);

#endif