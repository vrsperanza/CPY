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
		line[i] = ' ';
		stringInsert(line, "std::cout", i);
		i += 10;
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
		line[i] = ' ';
		stringInsert(line, "std::cout", i);
		i += 10;
		
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
	if(line[pos+4] != ' ' && line[pos+4] != '\t')
		return false;
	if(pos > 0 && line[pos-1] != ' ' && line[pos-1] != '\t' && line[pos-1] != ':')
		return false;
	if(pos == -1)
		return false;
	
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

bool cinPrintParse(char * line){
	int pos = string_isSubstring(line, "cin");
	
	if(line[pos+3] != ' ' && line[pos+3] != '\t')
		return false;
	if(pos > 0 && line[pos-1] != ' ' && line[pos-1] != '\t' && line[pos-1] != ':')
		return false;
	if(pos == -1)
		return false;
	
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

void joinContinuedLines(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		for(int i = 0; line[i] != '\0';){
			int j = i+1;
			while(stringContainsChar(" \t", line[j])) j++;
			if(line[i] == '\\' && line[j] == '\n'){
				line[i] = ' ';
				line[j] = ' ';
			}
			i = j;
		}
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return;
	
}

void formatLineSpacing(char * line){
	int resultPos = 0;
	int i = 0;
	while(line[i] == ' ' || line[i] == '\t')
		line[resultPos++] = line[i++];
	
	while(line[i] != '\0'){
		line[resultPos++] = line[i++];
		if(line[i] == ' ' || line[i] == '\t'){
			line[resultPos++] = ' ';
			while(line[i] == ' ' || line[i] == '\t')
				i++;
		}
	}
	if(line[i-1] == ' ')
		line[resultPos-1] = '\0';
	else
		line[resultPos] = '\0';
}

void implyWordSeparationSpaces(char * line){
	if(line[0] == '#')
		return;
	
	string resultString = "";
	for(int i = 0; line[i] != '\0'; i++){
		
		if(line[i] == '\"' && line[i-1] != '\\'){
			resultString += line[i++];
			while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0')
				resultString += line[i++];
		}
		
		int j = i;
		while(stringContainsChar("+-*/%=<>&^|!?,", line[j]))
			j++;
		
		if(i != j){
			char last;
			char next;
			int k = i-1;
			while((line[k] == ' ' || line[k] == '\t') && k >= 0)
				k--;
			if(k != -1)
				last = line[k];
			else
				last = '\0';
			
			bool isOperator = !stringContainsChar("+-*/%=<>&^|!?,;.({[", last) && last != '\0';
			
			
			if(	isOperator && 
				(line[i] != '/' || line[i+1] != '/') &&
				(line[i] != '/' || line[i+1] != '*') &&
				(line[i] != '*' || line[i+1] != '/') &&
				(line[i] != '-' || line[i+1] != '>') &&
				line[i] != ',')
				resultString += ' ';
			
			
			k = i;
			while(k < j)
				resultString += line[k++];
			
			if(	isOperator && 
				(line[i] != '/' || line[i+1] != '/') &&
				(line[i] != '/' || line[i+1] != '*') &&
				(line[i] != '*' || line[i+1] != '/') &&
				(line[i] != '-' || line[i+1] != '>'))
				resultString += ' ';
				
			i = k-1;
		}
		else{
			resultString += line[i];
		}
	}
	
	strcpy(line, resultString.c_str());
}

void formatSpacing(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		implyWordSeparationSpaces(line);
		formatLineSpacing(line);
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return;
}

void firstReplaces(const char * filename){
	joinContinuedLines(filename);
	replaceRawIncludes(filename);
	replaceQuickPrints(filename);
	formatSpacing(filename);
}