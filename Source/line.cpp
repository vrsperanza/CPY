#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <unordered_set>
#include "string.h"
#include "defines.h"
using namespace std;

vector<string> splitWords(char * line, string firstWordRequired="", string separators=wordSeparators){
	vector<string> words;
	int pos = 0;
	if(line[pos] != '\0'){
		string word;
		while(stringContainsChar(separators, line[pos])) pos++;
		while(line[pos] != '\0' && !stringContainsChar(separators, line[pos])) word += line[pos++];
		
		if(firstWordRequired != "" && word != firstWordRequired)
			return words;
		
		words.push_back(word);
	}
	
	while(line[pos] != '\0'){
		string word;
		while(stringContainsChar(separators, line[pos])) pos++;
		while(line[pos] != '\0' && !stringContainsChar(separators, line[pos])) word += line[pos++];
		words.push_back(word);
	}
	return words;
}

int stringClosePosition(char * line, int startPos){
	for(int i = startPos+1; line[i] != '\0'; i++)
		if(line[i] == '\"' && line[i-1] != '\\')
			return i;
	return -1;
}

int closePosition(char * line, int startPos, char open, char close){
	int count = 0;
	for(int i = startPos; line[i] != '\0'; i++){
		if(line[i] == open)
			count++;
		else if(line[i] == close)
			count--;
		if(count == 0)
			return i;
	}
	return -1;
}

vector<string> smartSplitWords(char * line, string firstWordRequired="", string separators=wordSeparators, bool keepSplits=false){
	vector<string> words;
	int pos = 0;
	string word;
	
	bool firstWordRequiredDone = (firstWordRequired == "");
	
	while(line[pos] != '\0'){
		word = "";
		if(keepSplits){
			while(stringContainsChar(separators, line[pos]))
				word += line[pos++];
			words.push_back(word);
		} else {
			while(stringContainsChar(separators, line[pos])) pos++;
		}
		
		word = "";
		while(line[pos] != '\0' && !stringContainsChar(separators, line[pos])){
			int pos2 = pos;
			
			if(line[pos] == '(')
				pos2 = closePosition(line, pos, '(', ')');
			else if(line[pos] == '[')
				pos2 = closePosition(line, pos, '[', ']');
			else if(line[pos] == '{')
				pos2 = closePosition(line, pos, '{', '}');
			else if(line[pos] == '\"')
				pos2 = stringClosePosition(line, pos);
			
			while(pos <= pos2)
				word += line[pos++];
		}
		
		if(!firstWordRequiredDone && firstWordRequired != word)
			return vector<string>();
		else
			firstWordRequiredDone = true;
		
		if(word != "")
			words.push_back(word);
		else
			break;
	}
	return words;
}

string getFirstWord(char * line, string separators=wordSeparators){
	string word = "";
	int i = 0;
	while(line[i] == ' ' || line[i] == '\t') i++;
	while(line[i] != '\0' && !stringContainsChar(separators, line[i])) word += line[i++];
	return word;
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

bool isNamespaceDeclaration(char * line){
	return smartSplitWords(line, "using").size() > 0;
}

bool isFunctionDeclaration(char * line){
	int whitespaceCount = 0;
	int openDec = 0;
	while(1){
		while(stringContainsChar(" \t\r\n)[]{},.;!@#%^&*-=+/:\"\'\\", line[openDec])) openDec++;
		if(line[openDec] == '(')
			break;
		
		whitespaceCount++;
		
		while(!stringContainsChar(wordSeparators, line[openDec]) && line[openDec] != '\0') openDec++;
		if(line[openDec] == '(')
			break;
		
		if(line[openDec] == '\0')
			return false;
	}
	
	if(whitespaceCount == 2){
		int len = strlen(line);
		len--;
		while(len >= 0 && line[len] != '{') len--;
		if(len >= 0 && line[len] == '{'){
			if( string_isWord(line, "do") != -1 ||
				string_isWord(line, "else") != -1 ||
				string_isWord(line, "if") != -1 ||
				string_isWord(line, "class") != -1 ||
				string_isWord(line, "struct") != -1 ||
				string_isWord(line, "typedef") != -1)
				return false;
			return true;
		}
		else return false;
	}
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

string rawInclude(char * line){
	char compString[] = "#raw";
	int i;
	for(i = 0; compString[i] == line[i] && compString[i] != '\0' && line[i] != '\0'; i++);
	
	string includeName = "";
	if(i != 4)
		return includeName;
	while(line[i] == ' ' || line[i] == '\t') i++;
	if(line[i] == '\"'){
		i++;
		while(line[i] != '\"' && line[i] != '\0') includeName += line[i++];
		return includeName;
	}
	return includeName;
}