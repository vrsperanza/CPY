#include <string>
#include <string.h>
#include <vector>
using namespace std;
#include "extensionHandler.h"
#include "globalHeader.h"
#include "line.h"

vector<string> getDependencies(char * inputFile){
	vector<string> dependencies;
	
	FILE * input = fopen(inputFile, "r");
	
	char inputFileName[100];
	strcpy(inputFileName, inputFile);
	removeExtension(inputFileName);

	int aux, i;
	
	int bracketsCount = 0;
	char buff[LINESZ];
	int buffLen;
	
	bool multiLineComment = false;
	
	while (fgets (buff, LINESZ, input)) {
		for(i = 0; buff[i] != '\0'; i++){
			if(buff[i] == '/' && buff[i+1] == '*') multiLineComment = true;
			else if(buff[i] == '*' && buff[i+1] == '/') multiLineComment = false;
		}
		
		if(!multiLineComment){
			int includePos = internalInclude(buff);
			if(includePos != -1){
				string depName = "";
				int j = 0;
				for(int i = includePos + 1; buff[i] != '.' && buff[i] != '\"' && buff[i] != '\0'; i++)
					depName += buff[i];
				dependencies.push_back(depName);
			}
		}
	}
	fclose(input);
	
	return dependencies;
}