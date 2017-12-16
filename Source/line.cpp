#include <string>
#include <string.h>
#include <vector>
#include <unordered_set>
#include "string.h"
using namespace std;

#include "globalHeader.h"

void addPahrenthesis(char * s){	
	int offset = 0;
	for(offset = 0; stringContainsChar(" \n\t", s[offset]); offset++)
		if(s[offset+1] == '/' && s[offset+2] == '/')
			while(s[offset] != '\n')
				offset++;
	
	if(string_isSubstring(s, "struct") >= 0)
		return;
	
	int len = strlen(s);
	
	if(s[len-2] == '{'){
		int lookStart = max(max(max(
			string_isSubstring(s, "if"),
			string_isSubstring(s, "for")),
			string_isSubstring(s, "while")),
			string_isSubstring(s, "switch"));
		
		if(lookStart == -1){
			implyFunctionParametersType(s);
		}
		else{
			int i = lookStart;
			while(stringContainsChar(" \t\n", s[i]))
				i++;
			
			bool imply = true;
			
			while(!stringContainsChar(" \t\n(", s[i]) && s[i] != '\0') i++;
			while(stringContainsChar(" \t\n", s[i])) i++;
			
			int parenthesisCount = 0;
			for(; s[i] != '\0'; i++){
				if(s[i] == '(')
					parenthesisCount++;
				else if(s[i] == ')')
					parenthesisCount--;
				
				if(parenthesisCount == 0){
					if(s[i] == ')')
						i++;
					while(stringContainsChar(" \t\n{;", s[i]))
						i++;
					imply = !(s[i] == '\0');
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