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

#include <iostream>

bool replaceRawIncludesInner(const char * filename){	
	bool replacedSomething = false;
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return replacedSomething;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	char line2[LINESZ];
	while (fgets (line, LINESZ, input)) {
		string includeName = rawInclude(line);
		if(includeName != ""){
			replacedSomething = true;
			
			//Keep whatever is after #raw "file.ext"
			//Eg:
			//#raw "template.cpy" //AutoTag
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
			
			//Copies include content to current file
			if(fileExist(includeName.c_str())){
				FILE * copy = fopen(includeName.c_str(), "r");
				while (fgets (line2, LINESZ, copy)){
					fprintf(output, "%s", line2);
				}
				fprintf(output, "\n");
				fclose(copy);
			}
			else {
				printf("Raw include file not found: %s", includeName);
			}
		}
		else
			fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return replacedSomething;
}

void replaceRawIncludes(const char * filename){
	int timeout = 1000;
	while(replaceRawIncludesInner(filename) && timeout--);
	if(timeout <= 0)
		printf("Too many iterations on raw replacement | File: %s\n", filename);
}
