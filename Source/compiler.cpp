#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <vector>
#include <set>
#include <map>
#include <stack>

#include "parser.h"
#include "header.h"
#include "compiler.h"

using namespace std;
char path[LINESZ] = "";

bool fileExist (char * fileName){
	if(access( fileName, F_OK ) != -1)
		return true;
	return false;
}

string removeCharExt(char * fileName){
	string s = "";
	for(int i = 0; fileName[i] != '.' && fileName[i] != '\0'; i++){
		s += fileName[i];
	}
	return s;
}

void stringToCPY(string input, char * fileName){
	int i;
	strcpy(fileName, path);
	int j = strlen(fileName);
	for(i = 0; i < input.size(); i++)
		fileName[j++] = input[i];
	fileName[j++] = '.';
	fileName[j++] = 'c';
	fileName[j++] = 'p';
	fileName[j++] = 'y';
	fileName[j++] = '\0';
}

void stringToCPP(string input, char * fileName){
	int i;
	strcpy(fileName, path);
	int j = strlen(fileName);
	for(i = 0; i < input.size(); i++)
		fileName[j++] = input[i];
	fileName[j++] = '.';
	fileName[j++] = 'c';
	fileName[j++] = 'p';
	fileName[j++] = 'p';
	fileName[j++] = '\0';
}

void stringToH(string input, char * fileName){
	int i;
	strcpy(fileName, path);
	int j = strlen(fileName);
	for(i = 0; i < input.size(); i++)
		fileName[j++] = input[i];
	fileName[j++] = '.';
	fileName[j++] = 'h';
	fileName[j++] = '\0';
}

void stringToLower(char * s){
	for(int i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

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

bool isOverwritable(char * fileName){
	FILE * f = fopen(fileName, "r");
	if(f == NULL)
		return true;
	
	char buff[LINESZ];
	fgets (buff, LINESZ, f);
	
	fclose(f);
	
	int len = strlen(buff);
	
	char autoTag[] = "AutoTag";
	
	int i, j;
	for(i = len - 8, j = 0; buff[i] != '\n' && buff[i] != '\0'; i++, j++)
		if(buff[i] != autoTag[j])
			return false;
	return true;
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
				printf("ERROR: Output flag requires filename\nEg: %s main.c -o execName", argv[0]);
				exit(0);
			}
			strcpy(target, argv[i + 1]);
			strcat(compilation, "-o ");
			strcat(compilation, argv[i + 1]);
			strcat(compilation, " ");
			i++;
		} else if(strcmp("-p", argument) == 0 || strcmp("-path", argument) == 0){
			strcpy(path, argv[++i]);
			strcat(path, "\\");
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
	
	string firstFile = "";
	for(int i = 0; source[i] != '.' && source[i] != '\0'; i++)
		firstFile += source[i];
	dependenciesToProcess.push(firstFile);
	
	char sourceFile[100];
	char cppFile[100];
	char headerFile[100];
	
	while(!dependenciesToProcess.empty()){
		string fileName = dependenciesToProcess.top();
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
				printf("Required file: %s not found", sourceFile);
		
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
