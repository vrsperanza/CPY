#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "globalHeader.h"
#include "extensionHandler.h"
#include "file.h"
#include "line.h"
#include "string.h"
#include "sourceGen.h"
using namespace std;

bool replaceRawIncludesInner(char * filename){	
	bool replacedSomething = false;
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return replacedSomething;
	
	FILE * output = fopen("temp.autocpp", "w+");
	char line[LINESZ];
	char line2[LINESZ];
	while (fgets (line, LINESZ, input)) {
		string includeName = rawInclude(line);
		if(includeName != ""){
			int aspCnt = 0;
			int i = 0;
			while(aspCnt < 2 && line[i] != '\0'){
				if(line[i] == '\"')
					aspCnt++;
				i++;
			}
			
			while(line[i] != '\0'){
				fprintf(output, "%c", line[i]);
				i++;
			}
			
			replacedSomething = true;
			char includeCpy[100];
			char includeCpp[100];
			char includeH[100];
			stringToCPY(includeName, includeCpy);
			stringToCPP(includeName, includeCpp);
			stringToH(includeName, includeH);
			
			if(fileExist(includeH)){
				FILE * copy = fopen(includeH, "r");
				while (fgets (line2, LINESZ, copy)){
					fprintf(output, "%s", line2);
				}
				fprintf(output, "\n");
				fclose(copy);
			}
			
			bool newGen = false;
			if(!fileExist(includeCpp) && fileExist(includeCpy)){
				generateSource(includeCpy, includeCpp, true);
				newGen = true;
			}
			
			if(fileExist(includeCpp)){
				FILE * copy = fopen(includeCpp, "r");
				while (fgets (line2, LINESZ, copy)){
					fprintf(output, "%s", line2);
				}
				fprintf(output, "\n");
				fclose(copy);
				
				if(newGen){
					remove(includeCpp);
				}
			}
			else {
				printf("Raw include file not found: %s", line);
			}
		}
		else
			fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename("temp.autocpp", filename);
	return replacedSomething;
}

void replaceRawIncludes(char * filename){
	int timeout = 1000;
	while(replaceRawIncludesInner(filename) && timeout--);
	if(timeout <= 0)
		printf("Too many iterations on raw replacement | File: %s\n", filename);
}
