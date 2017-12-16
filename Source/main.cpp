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
#include "folderHandler.h"

using namespace std;

void printHelp(){
	printf("USAGE: cpy SourceCode Flags\n");
	printf("Flags:\n");
	printf("\t-h: Shows this help page\n");
	printf("\t-b: Use line breaks on itermediate code\n");
	printf("\t-r: Automatically runs compiled code\n");
	printf("\t-nc: Doesn't compile resulting code\n");
	printf("\t-o target: Specifies target executable name, when not specified target = a\n");
	printf("\t-OtherFlags: Redirects flag to underlying compiler\n");
	exit(0);
}

int main(int argc, char ** argv){
	bool beauty = false;
	bool run = false;
	bool compile = true;
	
	int i, j;
	
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
		} else if(strcmp("-r", argument) == 0 || strcmp("-run", argument) == 0){
			run = true;
			compile = true;
		} else if(strcmp("-nc", argument) == 0|| strcmp("-nocompile", argument) == 0){
			compile = false;
			run = false;
		} else if(strcmp("-h", argument) == 0 || strcmp("-help", argument) == 0 || strcmp("-?", argument) == 0 || strcmp("?", argument) == 0 || strcmp("help", argument) == 0){
			printHelp();
		}  else {
			if(stringSource == "")
				if(argv[i][0] != '-')
					stringSource = removeCharExt(argv[i]);
				else{
					strcat(compilation, argv[i]);
					strcat(compilation, " ");
				}
			else
				printf("ERROR: Only one source file must be specified, ignoring %s\n", argv[i]);
		}
	}
	
	prepareFolder();
	chdir(compilationDirectory);
	stringSource = cropPath(stringSource);
	
	stack<string> dependenciesToProcess;
	set<string> filesDone;
	set<string> requiredHeaders;
	map<string, vector<string> > dependencies;
	
	dependenciesToProcess.push(stringSource);
	
	char cpyFile[100];
	char cppFile[100];
	char headerFile[100];
	
	while(!dependenciesToProcess.empty()){
		string fileName = dependenciesToProcess.top();
		dependenciesToProcess.pop();
		
		filesDone.insert(fileName);
			
		stringToCPY(fileName, cpyFile);
		stringToCPP(fileName, cppFile);
		
		vector<string> dependences;
		
		strcat(compilation, cppFile);
		strcat(compilation, " ");
		
		bool wroteCppFile = false;
		if(!fileExist(cppFile)){
			if(fileExist(cpyFile)){
				replaceRawIncludes(cpyFile);
				generateSource(cpyFile, cppFile, beauty);
			}
			else
				printf("Required file: %s not found\n", cpyFile);
		}
		
		dependences = getDependencies(cppFile);
		
		pair<string, vector<string> > dep;
		dep.first = fileName;
		dep.second = dependences;
		
		dependencies.insert(dep);
		
		for(string fileName : dependences){
			requiredHeaders.insert(fileName);
			
			if(!filesDone.count(fileName)){
				filesDone.insert(fileName);
				dependenciesToProcess.push(fileName);
			}
		}
	}

	for(string fileName : requiredHeaders){
		stringToH(fileName, headerFile);
		if(!fileExist(headerFile))
			generateHeader(cppFile, headerFile);
	}
	
	if(compile){
		system(compilation);
		string compiledFile = removeCharExt(target);
		compiledFile += ".exe";
		string parentTarget = "../" + compiledFile;
		rename(compiledFile.c_str(), parentTarget.c_str());
		
		if(run){
			printf("Running code:\n");
			system(parentTarget.c_str());
		}
	}
}
