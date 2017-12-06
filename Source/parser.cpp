#include "parser.h"

char stringif[] = "if";
char stringelse[] = "else";
char stringfor[] = "for";
char stringwhile[] = "while";
char stringswitch[] = "switch";
char stringStruct[] = "struct";
char stringTypedef[] = "typedef";

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

char strsub(char * a, char * b, int as){
	int bs = 0;
	while(a[as] != '\0' && b[bs] != '\0'){
		if(a[as++] != b[bs++])
			return 0;
	}
	return 1;
}

int string_isSubstring(char* mainStr, char* subStr){
    int aux;
    for(aux = 0; mainStr[aux] != '\0'; aux++){
        int aux1;
        for(aux1 = 0; subStr[aux1] == mainStr[aux+aux1]; aux1++){
            if(subStr[aux1+1] == '\0') return aux;
            if(mainStr[aux+aux1+1] == '\0') return -1;
        }
    }
    return -1;
}

void strInsert(char * dest, char * insert, int index){
	char destCpy[LINESZ];
	strcpy(destCpy, dest);
	int len = strlen(insert);
	int i;
	for(i = index; dest[i] != '\0'; i++){
		dest[i + len] = destCpy[i];
	}	
	dest[i + len + 1] = '\0';
	
	for(i = 0; insert[i] != '\0'; i++){
		dest[i+index] = insert[i];
	}
}

void implyFunctionParametersType(char * s){	
	char lastType[LINESZ] = "\0";
	int wordCount = 0;
	
	int typeStartIndex = 0;
	
	int i;
	for(i = 0; s[i] != '(' && s[i] != '\0'; i++);
	
	if(s[i] == '(') i++;
	
	while(s[i] == ' ' || s[i] == '\t') i++;
	
	int wordStartIndex = i;
	int wordEndIndex = i;
	int lastWordEndIndex = i;
	
	while(s[i] != '\0' && s[i] != '\n'){
		wordCount++;
		while(s[i] != ' ' && s[i] != '\t' && s[i] != '\0' && s[i] != '\n' &&  s[i] != ',' &&  s[i] != '(' &&  s[i] != '=') i++;
		
		lastWordEndIndex = wordEndIndex;
		wordEndIndex = i;
		
		while(s[i] == ' ' || s[i] == '\t') i++;
		
		if(s[i] == '=') while(!(s[i] == ',' || s[i] == '\n' || s[i] == '\0')) i++;
			
		
		if(s[i] == '(' || s[i] == ',' || s[i] == '\n' || s[i] == '\0'){
			if(wordCount == 1){
				strInsert(s, lastType, wordStartIndex);
				while(!(s[i] == '(' || s[i] == ',' || s[i] == '\n' || s[i] == '\0')) i++;
			} else if (wordCount > 1) {
				int j;
				for(j = wordStartIndex; j < lastWordEndIndex; j++){
					lastType[j-wordStartIndex] = s[j];
				}
				lastType[j-wordStartIndex] = ' ';
				lastType[j-wordStartIndex+1] = '\0';
			}
			wordCount = 0;
			i++;
			while(s[i] == ' ' || s[i] == '\t') i++;
			wordStartIndex = i;
		}
	}
}

void addPahrenthesis(char * s){
	int offset = 0;
	for(offset = 0; s[offset] == '\n' || s[offset] == '\t' || s[offset] == ' '; offset++)
		if(s[offset+1] == '/' && s[offset+2] == '/')
			while(s[offset] != '\n')
				offset++;
	
	if(string_isSubstring(s, stringStruct) >= 0)
		return;
	
	int len = strlen(s);
	
	if(s[len-2] == '{'){
		int lookStart = max(max(max(string_isSubstring(s, stringif), string_isSubstring(s, stringfor)), string_isSubstring(s, stringwhile)), string_isSubstring(s, stringswitch));
							
		if(lookStart == -1){
			implyFunctionParametersType(s);
		}
		else{
			
			int i;
			int parenthesisCount = 0;
			for(i = lookStart; s[i] != '\0'; i++){
				if(s[i] == '('){
					break;
				}
			}
			for(; s[i] != '\0'; i++){
				if(s[i] == '(')
					parenthesisCount++;
				else if(s[i] == ')')
					parenthesisCount--;
				
				if(parenthesisCount == 0)
					break;
			}
			
			bool imply = (s[i] == '\0');
			for(; s[i] != '\0'; i++){
				if(s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '{'){
					imply = true;
					break;
				}
			}
			
			if(imply){
				for(i = lookStart; s[i] != '\0'; i++){
					if(s[i] == ' ' || s[i] == '\t'){
						s[i] = '(';
						break;
					}
				}
				
				s[len-2] = ')';
				s[len-1] = '{';
				s[len] = '\n';
				s[len+1] = '\0';
			}
		}
	}
}

void getStructName(char * line, char * buffer){
	int spc = 0;
	int i, j;
	for(i = 0; line[i] != '\0' && spc < 2; i++){
		if(line[i] == ' ' || line[i] == '\t')
			spc++;
		while(line[i] == ' ' || line[i] == '\t') i++;
	}
	i--;
	
	if(spc != 2){
		printf("typedef struct name not properly specified\n%d\n", line);
		exit(0);
	}
	
	j = 0;
	while(line[i] != '\0' && line[i] != ' ' && line[i] != '\t' && line[i] != ';' && line[i] != '{'){
		buffer[j++] = line[i++];
	}
	buffer[j] = '\0';
}

bool isEmptyLine(char * line){
	for(int i = 0; line[i] != '\0' && !(line[i] == '/' && line[i+1] == '/'); i++){
		if(line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return false;
	}
	return true;
}

bool structHasTypedef(char * line){
	if(string_isSubstring(line, stringTypedef) >= 0)
		return true;
	return false;
}


void closeKeys(int offset, bool beauty){
	while(offset < offSets.top()){
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
	
	multiLineComment = false;
	
	inStruct = false;
	structTypedef = false;
	
	strcpy(buffPrevious, "");
	
	while (fgets (buff, LINESZ, input)) {
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
				offSets.push(offset);
				
				if(string_isSubstring(buffPrevious, stringStruct) >= 0){
					inStruct = true;
					if(string_isSubstring(buffPrevious, stringTypedef) >= 0){
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
			
			closeKeys(offset, beauty);			
		}
		
		addPahrenthesis(buffPrevious);
		fprintf(output, "%s", buffPrevious);
		
		for(i = 0; i < buffLen; i++)
			buffPrevious[i] = buff[i];
		for(; i < LINESZ; i++)
			buffPrevious[i] = 0;
		buffPreviousLen = buffLen;
    }
	
	closeKeys(0, beauty);
	addPahrenthesis(buffPrevious);
	fprintf(output, "%s", buffPrevious);
	fclose(input);
	fclose(output);
}