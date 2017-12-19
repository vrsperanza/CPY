#include <string>
#include <string.h>
using namespace std;

#include "defines.h"

char strsub(char * a, char * b, int as){
	int bs = 0;
	while(a[as] != '\0' && b[bs] != '\0'){
		if(a[as++] != b[bs++])
			return 0;
	}
	return 1;
}

int string_isSubstring(char* mainStr, string subStr){
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

void stringInsert(char * dest, string insert, int index){
	char destCpy[LINESZ];
	strcpy(destCpy, dest);
	int len = insert.size();
	int i;
	for(i = index; dest[i] != '\0'; i++){
		dest[i + len] = destCpy[i];
	}	
	dest[i + len] = '\0';
	for(i = 0; i < insert.size(); i++){
		dest[i+index] = insert[i];
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
	if(string_isSubstring(line, "typedef") >= 0)
		return true;
	return false;
}

bool stringContainsChar(string s, char check){
	for(char c : s)
		if(c == check)
			return true;
	return false;
}

void stringToLower(char * s){
	for(int i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

