#include "header.h"
#include "parser.h"

void removeExtension(char * s){
	int i;
	for(i = strlen(s); i >= 0 && s[i] != '.'; i--) s[i] = '\0';
	if(i != -1) s[i] = '\0';
}


void replaceHeaderExtension(char * s){
	int i;
	for(i = strlen(s); i >= 0 && s[i] != '.'; i--) s[i] = '\0';
	s[++i] = 'h';
	s[++i] = '\0';
}


bool isExternalInclude(char * line){
	char stringInclude[] = "#include";
	int i;
	for(i = 0; stringInclude[i] == line[i] && stringInclude[i] != '\0' && line[i] != '\0'; i++);
	
	if(i != 8)
		return false;
	while(line[i] == ' ' || line[i] == '\t') i++;
	if(line[i] == '<')
		return true;
	return false;
}

int internalInclude(char * line){
	char stringInclude[] = "#include";
	int i;
	for(i = 0; stringInclude[i] == line[i] && stringInclude[i] != '\0' && line[i] != '\0'; i++);
	
	if(i != 8)
		return -1;
	while(line[i] == ' ' || line[i] == '\t') i++;
	
	if(line[i] == '\"')
		return i;
	return -1;
}


int headersMade = 0;
void generateHeader(char * inputFile, char * outputFile){	
	FILE * input = fopen(inputFile, "r");
	FILE * output = fopen(outputFile, "w+");
	FILE * temp = fopen("temp.autocpp", "w+");
	char stringStruct[] = "struct";
	char stringTypedef[] = "typedef";
	
	char inputFileHeader[100];
	strcpy(inputFileHeader, inputFile);
	replaceHeaderExtension(inputFileHeader);
	
	char inputFileName[100];
	strcpy(inputFileName, inputFile);
	removeExtension(inputFileName);
	
	headersMade++;
	fprintf(output, "#ifndef _PROCEDURALHEADER%d_%s_H_ //AutoTag\n", headersMade, inputFileName);
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
		} else if(string_isSubstring(buff, stringStruct) != -1){
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
		} else if(string_isSubstring(buff, stringTypedef) != -1){
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
	
	//char bla[LINESZ];
	//strcpy(bla, inputFile);
	//strcat(bla, "temp");
	
	//remove(bla);
	//rename(inputFile, bla);
	
	remove(inputFile);
	rename("temp.autocpp", inputFile);
}

vector<string> getDependencies(char * inputFile){
	vector<string> dependencies;
	
	FILE * input = fopen(inputFile, "r");
	
	char inputFileName[100];
	strcpy(inputFileName, inputFile);
	removeExtension(inputFileName);

	int aux, i;
	
	int bracketsCount = 0;
	char buff[LINESZ];
	int buffLen;
	
	bool multiLineComment = false;
	
	while (fgets (buff, LINESZ, input)) {
		for(i = 0; buff[i] != '\0'; i++){
			if(buff[i] == '/' && buff[i+1] == '*') multiLineComment = true;
			else if(buff[i] == '*' && buff[i+1] == '/') multiLineComment = false;
		}
		
		if(!multiLineComment){
			int includePos = internalInclude(buff);
			if(includePos != -1){
				string depName = "";
				int j = 0;
				for(int i = includePos + 1; buff[i] != '.' && buff[i] != '\"' && buff[i] != '\0'; i++)
					depName += buff[i];
				dependencies.push_back(depName);
			}
		}
	}
	fclose(input);
	
	return dependencies;
}