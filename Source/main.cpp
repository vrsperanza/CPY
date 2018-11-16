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

#include "defines.h"
#include "sourceGen.h"
#include "headerGen.h"
#include "string.h"
#include "file.h"
#include "line.h"
#include "extensionHandler.h"
#include "dependenciesMapper.h"
#include "firstReplaces.h"
#include "directoryHandler.h"

using namespace std;

bool beauty = false;
bool run = false;
bool compile = true;
bool silent = false;
bool exportProject = false;
bool cleanCompilation = false;
bool implyVariables = true;

void printHelp(){
	printf("USAGE: cpy SourceCode Flags\n");
	printf("Flags:\n");
	printf("\t-h: Shows this help page\n");
	printf("\t-b: Use line breaks on itermediate code\n");
	printf("\t-s: Silent compilation, only prints errors\n");
	printf("\t-cl: Clean compilation, deletes CompilationSource before compiling\n");
	printf("\t-ex: Exports project to a directory containing only c++ source and a Makefile\n");
	printf("\t-r: Automatically runs compiled code\n");
	printf("\t-ni: Disables variable implication\n");
	printf("\t-nc: Doesn't compile resulting code\n");
	printf("\t-o target: Specifies target executable name, when not specified target = a\n");
	printf("\t-OtherFlags: Redirects flag to underlying compiler\n");
	exit(0);
}

void smartCompilation(map<string, vector<string> > dependenceMap, string mainFile, string target, string compilation, bool silent){
	set<string> generatedObjects;
	stack<string> toProcess;
	generatedObjects.insert(mainFile);
	toProcess.push(mainFile);
	string systemCall;
	
	bool someObjectUpdated = false;
	
	//Create / Update / Check Object files
	while(!toProcess.empty()){
		string fileName = toProcess.top();
		toProcess.pop();
		
		
		string cppSource = fileName + ".cpp";
		
		if(!fileExist(cppSource.c_str()))	
			continue;
		
		generatedObjects.insert(fileName);
		
		string targetObject = fileName + ".o";
		
		if(fileModifiedTime(targetObject.c_str()) < fileModifiedTime(cppSource.c_str())){
			systemCall = "g++ -c " + cppSource + " -o " + targetObject;
			
			if(!silent)
				cout << systemCall << endl;
			int returnValue = system(systemCall.c_str());
			if(returnValue != 0)
				return;
			
			
			//One object file was updated, linking has to happen again
			someObjectUpdated = true;
		}
		
		if(dependenceMap.count(fileName)){
			for(string dependence : dependenceMap[fileName]){
				if(!generatedObjects.count(dependence)){
					toProcess.push(dependence);
				}
			}
		}
		else
			printf("File not found: %s\n", fileName.c_str());
	}
	
	//Link all objects into the desired executable
	string trueTarget = "../" + target;
	if(someObjectUpdated || !fileExist((trueTarget + ".exe").c_str())){
		
		#ifdef _WIN32
		compilation += "-o " + trueTarget + " ";
		#else	
		compilation += "-o " + target + " ";
		#endif

		for(string object : generatedObjects){
			compilation += object + ".o ";
		}
		if(!silent)
			cout << compilation << endl;
		system(compilation.c_str());
		remove(trueTarget.c_str());
		rename(target.c_str(), trueTarget.c_str());
	} else {
		if(!silent)
			cout << target << " is up to date\n";
	}
}

int main(int argc, char ** argv){
	int i, j;
	
	char target[LINESZ] = "a";
	
	char compilation[LINESZ] = "g++ ";
	
	//Process Arguments
	if(argc < 2){
		printHelp();
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
			i++;
		} else if(strcmp("-b", argument) == 0 || strcmp("-beauty", argument) == 0 || strcmp("-toruncodes", argument) == 0){
			beauty = true;
		} else if(strcmp("-r", argument) == 0 || strcmp("-run", argument) == 0){
			run = true;
			compile = true;
		} else if(strcmp("-ni", argument) == 0|| strcmp("-noimplication", argument) == 0){
			implyVariables = false;
		} else if(strcmp("-nc", argument) == 0|| strcmp("-nocompile", argument) == 0){
			compile = false;
			run = false;
		} else if(strcmp("-s", argument) == 0|| strcmp("-silent", argument) == 0){
			silent = true;
		} else if(strcmp("-cl", argument) == 0 || strcmp("-clear", argument) == 0 || strcmp("-clean", argument) == 0){
			cleanCompilation = true;
		} else if(strcmp("-ex", argument) == 0|| strcmp("-export", argument) == 0){
			exportProject = true;
			beauty = true;
		} else if(strcmp("-h", argument) == 0 || strcmp("-help", argument) == 0 || strcmp("-?", argument) == 0 || strcmp("?", argument) == 0 || strcmp("help", argument) == 0 || strcmp("--help", argument) == 0){
			printHelp();
		} else {
			if(argv[i][0] != '-'){
				if(stringSource == "")
					stringSource = removeCharExt(argv[i]);
				else
					printf("ERROR: Only one source file must be specified, ignoring %s\n", argv[i]);
			} else {
				strcat(compilation, argv[i]);
				strcat(compilation, " ");
			}
		}
	}
	
	//Prepare compilation folder and move to it
	prepareDirectory(cleanCompilation);
	chdir(compilationDirectory);
	stringSource = cropPath(stringSource);
	
	//Generate complete c++ source
	stack<string> dependenciesToProcess;
	set<string> filesDone;
	set<string> requiredHeaders;
	set<string> allowedHeaders;
	map<string, vector<string> > dependenceMap;
	
	dependenciesToProcess.push(stringSource);
	
	char cpyFile[100];
	char cppFile[100];
	char headerFile[100];
	
	//Generate complete c++ source
	while(!dependenciesToProcess.empty()){
		string fileName = dependenciesToProcess.top();
		dependenciesToProcess.pop();
		
		filesDone.insert(fileName);
			
		stringToCPY(fileName, cpyFile);
		stringToCPP(fileName, cppFile);
		
		vector<string> dependencies;
		
		bool wroteCppFile = false;
		if(!fileExist(cppFile)){
			if(fileExist(cpyFile)){
				firstReplaces(cpyFile);
				generateSource(cpyFile, cppFile, beauty, implyVariables);
				allowedHeaders.insert(fileName);
			}
		} else {
			if(fileExist(cpyFile)){
				if(fileModifiedTime(cppFile) < fileModifiedTime(cpyFile)){
					remove(cppFile);
					firstReplaces(cpyFile);
					generateSource(cpyFile, cppFile, beauty, implyVariables);
					allowedHeaders.insert(fileName);
				}
			}
		}
		
		dependencies = getDependencies(cppFile);
		
		pair<string, vector<string> > dep;
		dep.first = fileName;
		dep.second = dependencies;
		
		dependenceMap.insert(dep);
		
		for(string fileName : dependencies){
			requiredHeaders.insert(fileName);
			
			if(!filesDone.count(fileName)){
				filesDone.insert(fileName);
				dependenciesToProcess.push(fileName);
			}
		}
	}

	//Generate necessary headers
	for(string fileName : requiredHeaders){
		if(allowedHeaders.count(fileName)){
			stringToCPP(fileName, cppFile);
			stringToH(fileName, headerFile);
			if(fileExist(headerFile))
				remove(headerFile);
			generateHeader(cppFile, headerFile);
		}
	}
	
	if(exportProject){
		createExportDirectory(filesDone, target);
	}
	
	//Compile files
	if(compile){		
		//Generate necessary object files and link
		smartCompilation(dependenceMap, stringSource, (string)target, (string)compilation, silent);
		
		if(run){
			if(!silent)
				printf("Running code:\n");
			system(((string)".." + (string)slash + (string)target).c_str());
		}
	}
}
