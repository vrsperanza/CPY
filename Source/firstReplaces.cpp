#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "defines.h"
#include "extensionHandler.h"
#include "file.h"
#include "line.h"
#include "string.h"
#include "sourceGen.h"
using namespace std;

#include <iostream>

bool exclamationPrintParse(char * line){
	int pos = string_isSubstring(line, "!");
	int i = pos;
	i--;
	while(stringContainsChar(" \t", line[i]) && i >= 0) i--;
	
	if((i == -1 || line[i] == '\n') && pos != -1){		
		int i = pos;
		line[i] = 's';
		stringInsert(line, "td::cout", i+1);
		i += 9;
		while(stringContainsChar(" \t\n", line[i]) && line[i] != '\0')
			i++;
		
		while(line[i] != '\n' && line[i] != '\0'){
			
			if(line[i] == '\"' && line[i-1] != '\\'){
				i++;
				while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0') i++;
			}
			
			while(!stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
			
			int insertPos = i;
			
			while(stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
			
			
			if(line[i] != '\n' && line[i] != '\0'){
				stringInsert(line, " \" \"", insertPos);
				i += 4;
			}
		}
		
		if(line[i-1] == '\n')
			i--;
		stringInsert(line, " std::endl", i);
		return true;
	}
	else return false;
}

bool interrogationPrintParse(char * line){
	int pos = string_isSubstring(line, "?");
	int i = pos;
	i--;
	while(stringContainsChar(" \t", line[i]) && i >= 0) i--;
	
	if((i == -1 || line[i] == '\n') && pos != -1){
		i = pos;
		line[i] = 's';
		stringInsert(line, "td::cout", i+1);
		i += 9;
		
		bool first = true;
		while(line[i] != '\n' && line[i] != '\0'){
			while(stringContainsChar(" \t", line[i]) && line[i] != '\0') i++;
			if(line[i] == '\n')
				break;
			
			string toClone = "";
			int insertPos = i;
			while(!stringContainsChar(" \t\n", line[i]) && line[i] != '\0'){
			
				if(line[i] == '\"' && line[i-1] != '\\'){
					i++;
					while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0') {
						toClone += line[i];
						i++;
					}
					if(line[i] == '\"')
						i++;
				} else {
					toClone += line[i];
					i++;
				}
			}
			
			toClone += " = \" ";
			
			if(first){
				toClone.insert(0, "\"");
				first = false;
			}
			else
				toClone.insert(0, "\" | ");
			
			stringInsert(line, toClone, insertPos);
			i += toClone.size();
		}
		
		while(line[i] != '\n' && line[i] != '\0')
			i++;
		
		stringInsert(line, " std::endl", i);
		return true;
	}
	else return false;
}

bool coutPrintParse(char * line){
	int pos = string_isSubstring(line, "cout");
	
	if(pos != -1){
		for(int i = 0; line[i] != '\0'; i++){
			if(line[i] == '\"'){
				i++;
				while(line[i] != '\"' && line[i] != '\0') i++;
			}
			
			if(line[i] == '<' && line[i+1] == '<')
				return false;
		}
		
		int i = pos;
		
		while(line[i] != '\n' && line[i] != '\0'){
			
			if(line[i] == '\"' && line[i-1] != '\\'){
				i++;
				while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0') i++;
			}
			
			while(!stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
			
			int insertPos = i;
			
			while(stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
			
			
			if(line[i] != '\n' && line[i] != '\0'){
				stringInsert(line, " <<", insertPos);
				i += 3;
			}
		}
		return true;
	}
	else return false;
}

bool cinPrintParse(char * line){
	int pos = string_isSubstring(line, "cin");
	
	if(pos != -1){
		for(int i = 0; line[i] != '\0'; i++){
			if(line[i] == '\"' && line[i-1] != '\\'){
				i++;
				while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0') i++;
			}
			
			if(line[i] == '>' && line[i+1] == '>')
				return false;
		}
		
		int i = pos;
		
		while(line[i] != '\n' && line[i] != '\0'){
			
			if(line[i] == '\"'){
				i++;
				while(line[i] != '\"' && line[i] != '\0') i++;
			}
			
			while(!stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
			
			int insertPos = i;
			
			while(stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
			
			
			if(line[i] != '\n' && line[i] != '\0'){
				stringInsert(line, " >>", insertPos);
				i += 3;
			}
		}
		return true;
	}
	else return false;
}

void replaceQuickPrints(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		
		//Must be in this order, interrogation and exclamation are parsed to semi-cout, which is in turn resolved by cout
		exclamationPrintParse(line);
		interrogationPrintParse(line);
		coutPrintParse(line);
		cinPrintParse(line);
		
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
}

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
