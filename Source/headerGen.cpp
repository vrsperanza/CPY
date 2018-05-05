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

void exportUntilKeyClose(FILE * input, FILE * output, FILE * lineBreakCompensation){
	int keyAmt = 1;
	char line[LINESZ];
	while (keyAmt && fgets (line, LINESZ, input)) {
		for(int i = 0; line[i] != '\0'; i++){
			if(line[i] == '{')
				keyAmt++;
			else if(line[i] == '}')
				keyAmt--;
		}
		fprintf(output, "%s", line);
		fprintf(lineBreakCompensation, "\n");
	}
}

void generateHeader(const char * inputFile, const char * outputFile){	
	FILE * input = fopen(inputFile, "r");
	FILE * header = fopen(outputFile, "w+");
	FILE * source = fopen(tempFile, "w+");
	
	char inputFileHeader[100];
	strcpy(inputFileHeader, inputFile);
	replaceHeaderExtension(inputFileHeader);
	
	char inputFileName[100];
	strcpy(inputFileName, inputFile);
	removeExtension(inputFileName);
	
	headersMade++;
	fprintf(header, "#ifndef _PROCEDURALHEADER%d_%s_H_\n", headersMade, inputFileName);
	fprintf(header, "#define _PROCEDURALHEADER%d_%s_H_\n", headersMade, inputFileName);
	
	char line[LINESZ];
	bool headerIncluded = false;
	while (fgets (line, LINESZ, input)) {
		bool toHeader = false;
		bool toSource = true;
		bool functionDeclaration = false;
		if(line[0] == '#' || string_isSubstring(line, "using namespace") != -1){
			toHeader = true;
			toSource = false;
		}
		else if(isFunctionDeclaration(line)){
			toHeader = true;
			toSource = true;
			functionDeclaration = true;
		}
		else if(string_isWord(line, "struct") != -1 || string_isWord(line, "class") != -1){
			int len = strlen(line);
			for(len = len-1; stringContainsChar(" \t\n", line[len]); len--);
			if(len != -1 && line[len] == '{'){
				fprintf(header, "%s", line);
				exportUntilKeyClose(input, header, source);
				continue;
			}
		}
		
		if(toSource)
			fprintf(source, "%s", line);
		else if(!headerIncluded){
			headerIncluded = true;
			fprintf(source, "#include \"%s\"\n", inputFileHeader);
		} else
			fprintf(source, "\n");		
		
		if(toHeader){
			if(functionDeclaration){
				int l = strlen(line);
				for(l = l-1; l != -1 && line[l] != '{'; l--);
				if(l != -1)
					line[l] = ';';
			}
			
			fprintf(header, "%s", line);
		}
	}
	fprintf(header, "#endif");
	
	fclose(header);
	fclose(source);
	fclose(input);
	
	remove(inputFile);
	rename(tempFile, inputFile);
}