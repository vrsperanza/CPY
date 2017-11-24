#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "parser.h"
#include "header.h"
#include <vector>
#include <set>
#include <map>
#include <stack>

using namespace std;

#define LINESZ 1024
#define tabSpaces 4
char path[LINESZ] = "";

bool fileExist (char * filename){
	if(access( filename, F_OK ) != -1)
		return true;
	return false;
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
	printf("USAGE: cmp SourceCode Flags\n");
	printf("Flags:\n");
	printf("\t-h: Shows this help page\n");
	printf("\t-b: Use line breaks on itermediate code\n");
	printf("\t-k: Keeps intermediate code\n");
	printf("\t-r: Automatically runs compiled code\n");
	printf("\t-nc: Doesn't compile resulting code\n");
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
	char *file_contents;
	long input_file_size;
	FILE *input = fopen(fileName, "r");
	if(input == NULL){
		printf("COULD NOT OPEN FILE %s", fileName);
		return;
	}
	
	FILE *result = fopen("temp.autocpp", "w+");
	
	char buff[LINESZ];
	fgets (buff, LINESZ, input);
	
	char autoTag[] = " //AutoTag\n";
	
	int len = strlen(buff);
	if(buff[len-1] == '\n')
		buff[--len] = '\0';
	strcat(buff, autoTag);
	
	fprintf(result, "%s", buff);
	while (fgets (buff, LINESZ, input))
		fprintf(result, "%s", buff);
	
	fclose(input);
	fclose(result);
	
	remove(fileName);
	rename("temp.autocpp", fileName);
	return;
}

int main(int argc, char ** argv){	
	int i, j;
	
	char source[LINESZ];
	char target[LINESZ] = "a";
	bool beauty = false;
	bool clear = true;
	bool run = false;
	bool compile = true;
	
	char compilation[LINESZ] = "g++ ";
	
	if(argc < 2){
		printf("USAGE: %s SourceCode Flags", argv[0]);
		exit(0);
	}
	
	char argument[LINESZ];
	strcpy(argument, argv[1]);
	stringToLower(argument);
	if(strcmp("-h", argument) == 0 || strcmp("-help", argument) == 0 || strcmp("-?", argument) == 0 || strcmp("?", argument) == 0 || strcmp("help", argument) == 0){
		printHelp();
	}
		
	for(i = 2; i < argc; i++){
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
		} else if(strcmp("-nc", argument) == 0 || strcmp("-autocomp", argument) == 0 || strcmp("-nocompile", argument) == 0){
			compile = false;
			run = false;
		} else if(strcmp("-h", argument) == 0 || strcmp("-help", argument) == 0 || strcmp("-?", argument) == 0 || strcmp("?", argument) == 0 || strcmp("help", argument) == 0){
			printHelp();
		} else {
			strcat(compilation, argv[i]);
			strcat(compilation, " ");
		}
	}
	
	strcpy(source, argv[1]);
	
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
				wroteCppFile = true;
			}
			else
				printf("Required file: %s not found", sourceFile);
		
		if(wroteCppFile && isOverwritable(headerFile))
			if(fileExist(sourceFile))
				generateHeader(cppFile, headerFile);
			else
				printf("Required file: %s not found", sourceFile);
		
		if(wroteCppFile)
			placeAutoTag(cppFile);
		
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
	
	if(compile){
		printf("Compilation...\n");
		system(compilation);
		printf("Compilation complete\n");
	}
	
	if(clear){
		for(string fileName : filesDone){
			char cppFile[100];
			char headerFile[100];
			stringToCPP(fileName, cppFile);
			stringToH(fileName, headerFile);
			
			if(isOverwritable(cppFile))
				remove(cppFile);
			if(isOverwritable(headerFile))
				remove(headerFile);
		}
	}
	
	if(run){
		printf("Running code:\n");
		system(target);
	}
}