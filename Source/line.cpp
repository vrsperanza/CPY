#include <string>
#include <string.h>
#include <vector>
#include <unordered_set>
#include "string.h"
using namespace std;

#include "defines.h"

bool isExternalInclude(char * line){
	char stringInclude[] = "#include";
	int i;
	for(i = 0; stringInclude[i] == line[i] && stringInclude[i] != '\0' && line[i] != '\0'; i++);
	
	if(i != 8)
		return false;
	while(line[i] == ' ' || line[i] == '\t') i++;
	if(line[i] == '<')
		return true;
	return false;
}

int internalInclude(char * line){
	char stringInclude[] = "#include";
	int i;
	for(i = 0; stringInclude[i] == line[i] && stringInclude[i] != '\0' && line[i] != '\0'; i++);
	
	if(i != 8)
		return -1;
	while(line[i] == ' ' || line[i] == '\t') i++;
	
	if(line[i] == '\"')
		return i;
	return -1;
}

string rawInclude(char * line){
	char compString[] = "#raw";
	int i;
	for(i = 0; compString[i] == line[i] && compString[i] != '\0' && line[i] != '\0'; i++);
	
	string includeName = "";
	if(i != 4)
		return includeName;
	while(line[i] == ' ' || line[i] == '\t') i++;
	if(line[i] == '\"'){
		i++;
		while(line[i] != '\"' && line[i] != '\0') includeName += line[i++];
		return includeName;
	}
	return includeName;
}