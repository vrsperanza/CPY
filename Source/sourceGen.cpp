#include <unordered_set>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stack>
using namespace std;

#include "globalHeader.h"
#include "sourceGen.h"
#include "headerGen.h"
#include "string.h"
#include "file.h"
#include "line.h"
#include "extensionHandler.h"
#include "dependenciesMapper.h"
#include "rawIncludes.h"

FILE * input;
FILE * output;

int i, j;

stack<int> offSets;
bool multiLineComment;
char buff[LINESZ];
char buff2[LINESZ];
int buffLen;

char buffPrevious[LINESZ];
int buffPreviousLen;

bool inStruct;
bool structTypedef;
char structName[LINESZ];
std::vector<std::unordered_set<std::string> > seenWords;

void implyVariablesType(char * line){
	vector<string> words;
	string word = "";
	
	vector<int> autoInsertPositions;
	int wordStartIndex = 0;
	int lastWordStartIndex = 0;
	
	int lineSize = strlen(line);
	bool wordIsArray = false;
	for(int i = 0; i < lineSize; i++){
		if(stringContainsChar("[])*->.", line[i])){
			while(!stringContainsChar(" \t\n,;(){}", line[i]))
				i++;
			
			word = "";
		}
		
		if(line[i] == '/'){
			if(line[i+1] == '/'){
				i += 2;
				while(line[i-1] != '\n' && line[i] != '\0')
					i++;
			}
			if(line[i+1] == '*'){
				i += 2;
				while(line[i] != '\0' || (line[i-2] != '*' && line[i-1] != '/'))
					i++;
			}
		}
		
		
		
		if(line[i] == '\0')
			break;
		
		if(stringContainsChar(" \t\n,;(){}", line[i])){
			while(stringContainsChar(" \t\n,;(){}", line[i]))
				i++;
			
			//If only one word exists before equality and it hasn't been seen, imply type
			if(word == "="){
				bool wordSeen = false;
				for(unordered_set<string> wordsLayer : seenWords){
					if(wordsLayer.count(words.back())){
						wordSeen = true;
						break;
					}
				}
				
				if(!wordSeen){
					//No need to code type implication again since g++ does it once it sees the variable's type is "auto"
					//Inserts "auto" before variable name after reading all words
					
					int j = lastWordStartIndex-1;
					while((line[j] == ' ' || line[j] == '\t') && j >= 0)
						j--;
					
					//if last character before current word is a sequence ender, imply variable type
					if(j == -1 || stringContainsChar("\n,;(){}", line[j]))  
						autoInsertPositions.push_back(lastWordStartIndex);
				}
			}
			
			words.push_back(word);
			lastWordStartIndex = wordStartIndex;
			wordStartIndex = i;
			i--;
			word = "";
		}
		else {
			word += line[i];
		}
	}
	
	for(int i = autoInsertPositions.size()-1; i >= 0; i--)
		stringInsert(line, "auto ", autoInsertPositions[i]);
	
	//Add new words to seen to avoid duplicate declaration
	for(string word : words)
		seenWords.back().insert(word);
}

int closeKeys(int offset, bool beauty){
	int closeAmount = 0;
	while(offset < offSets.top()){
		closeAmount++;
		offSets.pop();				
		int retOffset = offSets.top();
		
		if(buffPrevious[buffPreviousLen-1] == '\n'){
			if(beauty){
				for(i = 0; i < retOffset/tabSpaces; i++)
					buffPrevious[buffPreviousLen++] = '\t';
				for(j = 0; j < retOffset%tabSpaces; j++)
					buffPrevious[buffPreviousLen++] = ' ';
				buffPrevious[buffPreviousLen++] = '}';
				buffPrevious[buffPreviousLen++] = '\n';
				buffPrevious[buffPreviousLen] = '\0';
			}
			else {
				buffPrevious[buffPreviousLen-1] = '}';
				buffPrevious[buffPreviousLen++] = '\n';
				buffPrevious[buffPreviousLen] = '\0';
			}
		}
		else {
			if(beauty){
				buffPrevious[buffPreviousLen++] = '\n';
				for(i = 0; i < retOffset/tabSpaces; i++)
					buffPrevious[buffPreviousLen++] = '\t';
				for(j = 0; j < retOffset%tabSpaces; j++)
					buffPrevious[buffPreviousLen++] = ' ';
				buffPrevious[buffPreviousLen++] = '}';
				buffPrevious[buffPreviousLen++] = '\n';
				buffPrevious[buffPreviousLen] = '\0';
			} else {
				buffPrevious[buffPreviousLen++] = '}';
				buffPrevious[buffPreviousLen] = '\0';
			}
		}
		
		if(inStruct){
			inStruct = false;
			if(structTypedef){
				structTypedef = false;
				i = 0;
				buffPrevious[buffPreviousLen-1] = ' ';
				while(structName[i] != '\0')
					buffPrevious[buffPreviousLen++] = structName[i++];
				buffPreviousLen++;
			}
			buffPrevious[buffPreviousLen - 1] = ';';
			buffPrevious[buffPreviousLen++] = '\n';
			buffPrevious[buffPreviousLen] = '\0';
		}
	}
	return closeAmount;
}

void generateSource(char * inputFile, char * outputFile, bool beauty){
	input = fopen(inputFile, "r");
	output = fopen(outputFile, "w+");
	
	if(input == NULL){
		printf("File not found\n");
		printf("USAGE: cmp SourceCode Flags");
		exit(0);
	}
	offSets.push(0);
	
	seenWords.clear();
	seenWords.push_back(unordered_set<string>());
	
	multiLineComment = false;
	
	inStruct = false;
	structTypedef = false;
	
	strcpy(buffPrevious, "");
	
	while (fgets (buff, LINESZ, input)) {
		
		
		//Skip empty lines
		bool empty = isEmptyLine(buff);
		while(empty){
			if(!fgets (buff2, LINESZ, input))
				break;
			if(buff[buffLen-2] == '\\'){
				buff[buffLen-2] = '\n';
				buff[buffLen-1] = '\0';
			}
			empty = isEmptyLine(buff2);
			strcat(buff, buff2);
			buffLen += strlen(buff2);
		}
		
		//Concat \ lines
		int buffLen = strlen(buff);
		while(buff[buffLen-2] == '\\'){
			fgets (buff2, LINESZ, input);
			if(buff[buffLen-2] == '\\'){
				buff[buffLen-2] = '\n';
				buff[buffLen-1] = '\0';
			}
			strcat(buff, buff2);
			buffLen += strlen(buff2);
		}
		
		
		
		
		//Read tab spaces
		int offset = 0;
		for(i = 0; i < buffLen; i++){
			if(buff[i] == '\t')
				offset += tabSpaces;
			else if(buff[i] == ' ')
				offset++;
			else if(buff[i] == '\n')
				offset = 0;
			else if(buff[i] == '/' && buff[i+1] == '/'){
				while(buff[i] != '\n' && buff[i] != '\0')
					i++;
				offset = 0;
			}
			else
				break;
		}
		
		bool emptyLine = (buff[i] == '\n');
		bool oneLineComment = false;
		bool cPreCompilerTag = false;
		bool multiLineCommentEnd = false;
		
		//Interpret comments & precompiler flags
		if(buff[i] == '#')
			cPreCompilerTag = true;
		else{
			for(; i < buffLen-1; i++){
				if(buff[i] == '/'){
					if(buff[i+1] == '/')
						oneLineComment = true;
					if(buff[i+1] == '*'){
						multiLineComment = true;
					}
				}
				
				if(buff[i] == '*' && buff[i+1] == '/'){
					multiLineComment = false;
					multiLineCommentEnd = true;
				}
			}
		}
		
		int outScopeAmount = 0;
		if(!multiLineComment && !multiLineCommentEnd && !cPreCompilerTag && !oneLineComment && !emptyLine){
			if(buff[i-1] != ';' && buff[i-1] != '/' && buff[i-1] != '\\'){
				if(buff[i] == '\n'){
					buff[i] = ';';
					buff[i+1] = '\n';
					buff[i+2] = '\0';
					buffLen++;
				}
				else {
					buff[i+1] = ';';
					buff[i+2] = '\0';
					buffLen++;
				}
			}
			
			if(offset > offSets.top()){
				//New scope
				seenWords.push_back(unordered_set<string>());
				
				offSets.push(offset);
				
				if(string_isSubstring(buffPrevious, "struct") >= 0){
					inStruct = true;
					if(string_isSubstring(buffPrevious, "typedef") >= 0){
						structTypedef = true;
						getStructName(buffPrevious, structName);
					}
					else structTypedef = false;
				}
				if(buffPrevious[buffPreviousLen-1] == '\n')
					buffPrevious[buffPreviousLen-2] = '{';
				else 
					buffPrevious[buffPreviousLen-1] = '{';
			} 
			
			outScopeAmount = closeKeys(offset, beauty);			
		}
		addPahrenthesis(buffPrevious);
		implyVariablesType(buffPrevious);
		while(outScopeAmount--)
			seenWords.pop_back();
		fprintf(output, "%s", buffPrevious);
		
		for(i = 0; i < buffLen; i++)
			buffPrevious[i] = buff[i];
		for(; i < LINESZ; i++)
			buffPrevious[i] = 0;
		buffPreviousLen = buffLen;
    }
	
	int outScopeAmount = closeKeys(0, beauty);
	addPahrenthesis(buffPrevious);
	implyVariablesType(buffPrevious);
	while(outScopeAmount--)
		seenWords.pop_back();
	fprintf(output, "%s", buffPrevious);
	fclose(input);
	fclose(output);
}