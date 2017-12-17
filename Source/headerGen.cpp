#include <vector>
#include <string>
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>

#include "defines.h"
#include "sourceGen.h"
#include "headerGen.h"
#include "string.h"
#include "file.h"
#include "line.h"
#include "extensionHandler.h"
#include "dependenciesMapper.h"

int headersMade = 0;
void generateHeader(char * inputFile, char * outputFile){	
	FILE * input = fopen(inputFile, "r");
	FILE * output = fopen(outputFile, "w+");
	FILE * temp = fopen(tempFile, "w+");
	
	char inputFileHeader[100];
	strcpy(inputFileHeader, inputFile);
	replaceHeaderExtension(inputFileHeader);
	
	char inputFileName[100];
	strcpy(inputFileName, inputFile);
	removeExtension(inputFileName);
	
	headersMade++;
	fprintf(output, "#ifndef _PROCEDURALHEADER%d_%s_H_\n", headersMade, inputFileName);
	fprintf(output, "#define _PROCEDURALHEADER%d_%s_H_\n", headersMade, inputFileName);

	int aux, i;
	
	int bracketsCount = 0;
	int isInsideStruct = 0;
	char buff[LINESZ];
	char nameBuff[100];
	int buffLen;
	
	bool multiLineComment = false;
	
	bool includedSelf = false;
	
	while (fgets (buff, LINESZ, input)) {
		bool keepLine = true;
		
		for(i = 0; buff[i] != '\0'; i++){
			if(buff[i] == '/' && buff[i+1] == '*') multiLineComment = true;
			else if(buff[i] == '*' && buff[i+1] == '/') multiLineComment = false;
		}
		
		if(bracketsCount == 0 && !includedSelf && !multiLineComment){
			for(i = 0; buff[i] == ' ' || buff[i] == '\t'; i++);
			if(buff[i] == '\n'){
				fprintf(temp, "#include \"%s\"", inputFileHeader);
				includedSelf = true;
				keepLine = false;
			}
		}
		
		if(!multiLineComment && isExternalInclude(buff)){
			fprintf(output, "%s", buff);
			keepLine = false;
		} else if(string_isSubstring(buff, "struct") != -1){
			int hasParentheses = 0;
			int hasBrackets = 0;
			for(aux = 0; buff[aux] != '\0'; aux ++){
				if(buff[aux] == '(' || buff[aux] == ')')
					hasParentheses = 1;
				else if(buff[aux] == '{')
					hasBrackets = 1;
			}
			if(hasParentheses == 0 && hasBrackets == 1){
				isInsideStruct = 1;
			}
		} else if(string_isSubstring(buff, "typedef") != -1){
			fprintf(output, "%s\n", buff);
			keepLine = false;
		}
		
		if(isInsideStruct == 1){
			keepLine = false;
			fprintf(output, "%s", buff);
			for(aux = 0;buff[aux] != '\0'; aux ++){
				if(buff[aux] == '}'){
					isInsideStruct = 0;
				}
			}
		}
		else{
			for(aux = 0; buff[aux] != '\0'; aux ++){
				if(aux > 0 && buff[aux] == '/' && buff[aux-1] == '/') break;
				else if(buff[aux] == '{'){
					if(bracketsCount == 0){
						
						char exportFunc[LINESZ];
						strcpy(exportFunc, buff);
						
						exportFunc[aux] = ';';
						for(int i = 0; buff[i] != '\0'; i++){
							if(buff[i] == '='){
								while(buff[i] != ',' && buff[i] != ')' && buff[i] != '{' && buff[i] != '\n' && buff[i] != '\0')
									buff[i++] = ' ';
							}
						}
						
						fprintf(output, "%s", exportFunc);
					}
					bracketsCount++;
				}
				else if(buff[aux] == '}'){
					bracketsCount--;
					if(bracketsCount < 0){
						exit(-1);
					}
				}
			}
		}
		
		
		if(keepLine){
			fprintf(temp, "%s", buff);
		} else {			
			if(!includedSelf && !multiLineComment){
				fprintf(temp, "#include \"%s\"", inputFileHeader);
				includedSelf = true;
			}			
			fprintf(temp, "\n");
		}
	}
	if(bracketsCount > 0){
		exit(-1);
	}
	fprintf(output, "#endif");	
	
	fclose(input);
	fclose(output);
	fclose(temp);
	
	remove(inputFile);
	rename(tempFile, inputFile);
}