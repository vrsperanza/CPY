#include <string>
using namespace std;

#include "defines.h"
#include "string.h"



bool stringContainsChar(const string& s, const char& check){
	for(char c : s)
		if(c == check)
			return true;
	return false;
}

bool stringContainsChars(const string& s1, const string& s2){
	for(char c : s1)
		if(s2.find_first_of(c) != string::npos)
			return true;
	return false;
}

bool isWhitespace(const string& s, string ignore){
	for(char c : s)
		if(ignore.find(c) == string::npos)
			return false;
	return true;
}

string removeTrailingWhitespace(string& s, string whitespace){
	int i = 0;
	while(stringContainsChar(whitespace, s[i])) i++;
	
	int j = s.size()-1;
	while(stringContainsChar(whitespace, s[j])) j--;
	return s.substr(i, j-i+1);
}

char strsub(const char* a, const char* b, int as){
	int bs = 0;
	while(a[as] != '\0' && b[bs] != '\0'){
		if(a[as++] != b[bs++])
			return 0;
	}
	return 1;
}

int string_isSubstring(const char* mainStr, const string& subStr){
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

int string_isWord(const char* mainStr, const string& subStr, string separators){
    int aux;
    for(aux = 0; mainStr[aux] != '\0'; aux++){
		if(aux-1 == -1 || stringContainsChar(separators, mainStr[aux-1])){
			int aux1;
			for(aux1 = 0; subStr[aux1] == mainStr[aux+aux1]; aux1++){
				if(subStr[aux1+1] == '\0')
					if(mainStr[aux+aux1+1] == '\0' || stringContainsChar(separators, mainStr[aux+aux1+1]))
						return aux;
				if(mainStr[aux+aux1+1] == '\0') return -1;
			}
		}
    }
    return -1;
}

void strInsert(char* dest, const char* insert, int index){
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

void stringInsert(char* dest, const string& insert, int index){
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

void getStructName(const char* line, char* buffer){
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

bool isEmptyLine(const char* line){
	for(int i = 0; line[i] != '\0' && !(line[i] == '/' && line[i+1] == '/'); i++){
		if(line[i] != ' ' && line[i] != '\t' && line[i] != '\n')
			return false;
	}
	return true;
}

bool structHasTypedef(const char* line){
	if(string_isSubstring(line, "typedef") >= 0)
		return true;
	return false;
}

void stringToLower(char * s){
	for(int i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}

