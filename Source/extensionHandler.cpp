#include <string>
#include <string.h>
using namespace std;

#include "defines.h"

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

string removeCharExt(char * fileName){
	string s = "";
	for(int i = 0; fileName[i] != '.' && fileName[i] != '\0'; i++){
		s += fileName[i];
	}
	return s;
}

string removeExtension(string fileName){
	string s = "";
	for(int i = 0; fileName[i] != '.' && fileName[i] != '\0'; i++){
		s += fileName[i];
	}
	return s;
}


void stringToCPY(string input, char * fileName){
	int i;
	int j = 0;
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
	int j = 0;
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
	int j = 0;
	for(i = 0; i < input.size(); i++)
		fileName[j++] = input[i];
	fileName[j++] = '.';
	fileName[j++] = 'h';
	fileName[j++] = '\0';
}


string getExtension(string input){
	int i;
	for(i = input.size()-1; i >= 0 && input[i] != '.'; i--);
	if(i == -1)
		return "";
	
	return input.substr(i, input.size()-i);
}

string cropPath(string input){
	int i;
	for(i = input.size()-1; i >= 0 && input[i] != '\\' && input[i] != '/'; i--);
	if(i == -1)
		return input;
	i++;
	return input.substr(i, input.size()-i);
}

bool isCodeFile(string input){
	string ext = getExtension(input);
	return (ext == ".cpy" || ext == ".cpp" || ext == ".h");
}

bool isCppFile(string input){
	string ext = getExtension(input);
	return (ext == ".cpp" || ext == ".h");
}