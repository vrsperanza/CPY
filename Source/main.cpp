#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <iostream>

#include "globalHeader.h"
#include "sourceGen.h"
#include "headerGen.h"
#include "string.h"
#include "file.h"
#include "line.h"
#include "extensionHandler.h"
#include "dependenciesMapper.h"
#include "rawIncludes.h"

using namespace std;

void printHelp(){
	printf("USAGE: cpy SourceCode Flags\n");
	printf("Flags:\n");
	printf("\t-h: Shows this help page\n");
	printf("\t-b: Use line breaks on itermediate code\n");
	printf("\t-k: Keeps intermediate code\n");
	printf("\t-r: Automatically runs compiled code\n");
	printf("\t-nc: Doesn't compile resulting code\n");
	printf("\t-nh: Doesn't create headers\n");
	printf("\t-o target: Specifies target executable name, when not specified target = a\n");
	printf("\t-OtherFlags: Redirects flag to underlying compiler\n");
	exit(0);
}

void placeAutoTag(char * fileName){		
	FILE *input = fopen(fileName, "r");
	if(input == NULL){
		printf("COULD NOT OPEN FILE %s", fileName);
		return;
	}
	
	FILE *result = fopen("temp.autocpp", "w+");
	
	char buff[LINESZ];
	fgets (buff, LINESZ, input);
	
	char autoTag[] = "//AutoTag\n";
	
	int len = strlen(buff);
	if(buff[len-1] == '\n')
		buff[--len] = '\0';
	strcat(buff, autoTag);
	
	fprintf(result, "%s", buff);
	while (fgets (buff, LINESZ, input)){
		fprintf(result, "%s", buff);
	}
	
	fclose(input);
	fclose(result);
	remove(fileName);
	rename("temp.autocpp", fileName);
	return;
}

int main(int argc, char ** argv){	

	bool beauty = false;
	bool clear = true;
	bool run = false;
	bool compile = true;
	bool generateHeaders = true;
	
	int i, j;
	
	char source[LINESZ];
	char target[LINESZ] = "a";
	
	char compilation[LINESZ] = "g++ ";
	
	if(argc < 2){
		printf("USAGE: %s SourceCode Flags\n", argv[0]);
		exit(0);
	}
	
	char argument[LINESZ];
	string stringSource = "";
	for(i = 1; i < argc; i++){
		strcpy(argument, argv[i]);
		stringToLower(argument);
		
		if(strcmp("-o", argument) == 0){
			if(i + 1 >= argc){
				printf("ERROR: Output flag requires filename\nEg: %s main.c -o execName\n", argv[0]);
				exit(0);
			}
			strcpy(target, argv[i + 1]);
			strcat(compilation, "-o ");
			strcat(compilation, argv[i + 1]);
			strcat(compilation, " ");
			i++;
		} else if(strcmp("-b", argument) == 0 || strcmp("-beauty", argument) == 0 || strcmp("-toruncodes", argument) == 0){
			beauty = true;
			clear = false;
		} else if(strcmp("-k", argument) == 0 || strcmp("-keep", argument) == 0){
			clear = false;
		} else if(strcmp("-r", argument) == 0 || strcmp("-run", argument) == 0){
			run = true;
			compile = true;
		} else if(strcmp("-nc", argument) == 0|| strcmp("-nocompile", argument) == 0){
			compile = false;
			run = false;
		} else if(strcmp("-nh", argument) == 0 || strcmp("-noheader", argument) == 0 || strcmp("-?", argument) == 0 || strcmp("?", argument) == 0 || strcmp("help", argument) == 0){
			generateHeaders = false;
		}else if(strcmp("-h", argument) == 0 || strcmp("-help", argument) == 0 || strcmp("-?", argument) == 0 || strcmp("?", argument) == 0 || strcmp("help", argument) == 0){
			printHelp();
		}  else {
			if(stringSource == "")
				stringSource = removeCharExt(argv[i]);
			else
				printf("ERROR: Only one source file must be specified, ignoring %s\n", argv[i]);
		}
	}
	
	stringToCPP(stringSource, source);
	
	stack<string> dependenciesToProcess;
	set<string> filesDone;
	map<string, vector<string> > dependencies;
	
	dependenciesToProcess.push(stringSource);
	
	char sourceFile[100];
	char cppFile[100];
	char headerFile[100];
	
	while(!dependenciesToProcess.empty()){
		string fileName = dependenciesToProcess.top();
		cout << "Process: " << fileName << endl;
		dependenciesToProcess.pop();
		
		filesDone.insert(fileName);
			
		stringToCPY(fileName, sourceFile);
		stringToCPP(fileName, cppFile);
		stringToH(fileName, headerFile);
		
		vector<string> dependence;
		
		strcat(compilation, cppFile);
		strcat(compilation, " ");
		
		bool wroteCppFile = false;
		if(isOverwritable(cppFile))
			if(fileExist(sourceFile)){
				generateSource(sourceFile, cppFile, beauty);
				if(isOverwritable(headerFile) && generateHeaders)
					generateHeader(cppFile, headerFile);
				placeAutoTag(cppFile);
			}
			else
				printf("Required file: %s not found\n", sourceFile);
		
		dependence = getDependencies(cppFile);
		
		pair<string, vector<string> > dep;
		dep.first = fileName;
		dep.second = dependence;
		
		dependencies.insert(dep);
		
		for(string fileName : dependence){
			if(!filesDone.count(fileName)){
				filesDone.insert(fileName);
				dependenciesToProcess.push(fileName);
			}
		}
	}
	
	for(string fileName : filesDone){
		char cppFile[100];
		char headerFile[100];
		stringToCPP(fileName, cppFile);
		stringToH(fileName, headerFile);
		
		replaceRawIncludes(cppFile);
		replaceRawIncludes(headerFile);
	}

	if(compile){
		system(compilation);
	}
	
	if(clear){
		for(string fileName : filesDone){
			char cppFile[100];
			char headerFile[100];
			stringToCPP(fileName, cppFile);
			stringToH(fileName, headerFile);
			
			if(fileExist(cppFile) && isOverwritable(cppFile))
				remove(cppFile);
			if(fileExist(headerFile) && isOverwritable(headerFile))
				remove(headerFile);
		}
	}
	
	if(run){
		printf("Running code:\n");
		system(target);
	}
}
