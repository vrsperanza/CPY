#include <string>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stack>
#include <vector>

#include "defines.h"
#include "extensionHandler.h"
#include "file.h"
#include "line.h"
#include "string.h"
#include "sourceGen.h"
using namespace std;

#include <iostream>

void treatLineEndings(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		int resultPos = 0;
		int pos = 0;
		while(line[pos] != '\0'){
			if(line[pos] == '\r'){
				line[resultPos++] = '\n';
				pos += 1 + (line[pos+1] == '\n');
			}
			else
				line[resultPos++] = line[pos++];
		}
		
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return;
}

void forLoopParse(char * line){
	vector<string> words = smartSplitWords(line, "for", " \t\n,;()=", true);
	if(words.size() == 0)
		return;
	
	if(isWhitespace(words[words.size()-1], " \t\n,;()")) words.pop_back();
	
	string offSet = words[0];
	string i;
	string n;
	string equity = "0";
	
	for(int i = 0; i < words.size(); i++){
		if(removeTrailingWhitespace(words[i], " \t\n,;()") == "="){
			if(words.size() <= i+1){
				printf("LINE: %s ends in incomplete equity", line);
				exit(0);
			}
			equity = words[i+1];
			words.erase(words.begin()+i,words.begin()+i+2);
			break;
		}
	}
	
	if(words.size() == 6){
		i = words[3];
		n = words[5];
		string result = "";
		int pos = 0;
		while(line[pos] == ' ' || line[pos] == '\t') result += line[pos++];
		
		result += "for(" + i + " = " + equity + "; " + i + " < " + n + "; " + i + "++)\n";
		
		strcpy(line, result.c_str());
		return;
	}
}

void rofLoopParse(char * line){
	vector<string> words = smartSplitWords(line, "rof", " \t\n,;()=", true);
	if(words.size() == 0)
		return;
	
	if(isWhitespace(words[words.size()-1], " \t\n,;()")) words.pop_back();
	
	string offSet = words[0];
	string i;
	string n;
	string equity = "0";
	
	for(int i = 0; i < words.size(); i++){
		if(removeTrailingWhitespace(words[i], " \t\n,;()") == "="){
			if(words.size() <= i+1){
				printf("LINE: %s ends in incomplete equity", line);
				exit(0);
			}
			equity = words[i+1];
			words.erase(words.begin()+i,words.begin()+i+2);
			break;
		}
	}
	
	if(words.size() == 6){
		i = words[3];
		n = words[5];
		string result = "";
		int pos = 0;
		while(line[pos] == ' ' || line[pos] == '\t') result += line[pos++];
		
		result += "for(" + i + " = " + n + " - 1; " + i + " >= " + equity + "; " + i + "--)\n";
		
		strcpy(line, result.c_str());
		return;
	}
}

void exclamationPrintParse(char * line){
	vector<string> words = smartSplitWords(line, "!", " \t\n,;", true);
	if(words.size() > 0){		
		string s = words[0] + "std::cout";
		bool first = true;
		for(int i = 3; i < words.size(); i += 2){
			if(!first)
				s += " << \" \"";
			first = false;
			s += " << " + words[i];
		}
		s += " << std::endl\n";
		strcpy(line, s.c_str());
		return;
	}
	
	words = smartSplitWords(line, "!!", " \t\n,;", true);
	if(words.size() > 0){
		string s = words[0] + "std::cout";
		bool first = true;
		for(int i = 3; i < words.size(); i += 2){
			if(!first)
				s += " << \" \"";
			first = false;
			s += " << " + words[i];
		}
		s += '\n';
		strcpy(line, s.c_str());
		return;
	}
}

void interrogationPrintParse(char * line){
	vector<string> words = smartSplitWords(line, "?", " \t\n,;", true);
	if(words.size() > 0){
		string s = words[0];
		char last = 'x';
		for(int i = 3; i < words.size(); i += 2){
			if(words[i][0] == '\"'){
				if(last == 'o')
					s += " << " + words[i];
				else if(last == 'x')
					s += "std::cout << " + words[i];
				else
					if(beauty)
						s += "\n" + words[0] + "std::cout << " + words[i];
					else
						s += "; std::cout << " + words[i];
				last = 'o';
			}
			else{
				if(last == 'i')
					s += " >> " + words[i];
				else if(last == 'x')
					s += "std::cin >> " + words[i];
				else
					if(beauty)
						s += "\n" + words[0] + "std::cin >> " + words[i];
					else
						s += "; std::cin >> " + words[i];
				last = 'i';
			}
		}
		s += '\n';
		strcpy(line, s.c_str());
		return;
	}
	
	words = smartSplitWords(line, "??", " \t\n,;", true);
	if(words.size() > 0){
		string s = words[0] + "std::cout";
		bool first = true;
		for(int i = 3; i < words.size(); i += 2){
			if(first){
				s += " << \"" + words[i] + " = \" << " + words[i];
				first = false;
			} else
				s += " << \" | " + words[i] + " = \" << " + words[i];
		}
		s += " << std::endl\n";
		strcpy(line, s.c_str());
		return;
	}
}

bool coutPrintParse(char * line){
	int pos = max(	string_isSubstring(line, "cout"),
					string_isSubstring(line, "cerr"));
	if(line[pos+4] != ' ' && line[pos+4] != '\t')
		return false;
	if(pos > 0 && line[pos-1] != ' ' && line[pos-1] != '\t' && line[pos-1] != ':')
		return false;
	if(pos == -1)
		return false;
	
	for(int i = 0; line[i] != '\0'; i++){
		if(line[i] == '\"'){
			i++;
			while(line[i] != '\"' && line[i] != '\0') i++;
		}
		
		if(line[i] == '<' && line[i+1] == '<')
			return false;
	}
	
	int i = pos;
	
	while(line[i] != '\n' && line[i] != '\0'){
		
		if(line[i] == '\"' && line[i-1] != '\\'){
			i++;
			while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0') i++;
		}
		
		while(!stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
		
		int insertPos = i;
		
		while(stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
		
		
		if(line[i] != '\n' && line[i] != '\0'){
			stringInsert(line, " <<", insertPos);
			i += 3;
		}
	}
	return true;
}

bool cinPrintParse(char * line){
	int pos = string_isSubstring(line, "cin");
	
	if(line[pos+3] != ' ' && line[pos+3] != '\t')
		return false;
	if(pos > 0 && line[pos-1] != ' ' && line[pos-1] != '\t' && line[pos-1] != ':')
		return false;
	if(pos == -1)
		return false;
	
	for(int i = 0; line[i] != '\0'; i++){
		if(line[i] == '\"' && line[i-1] != '\\'){
			i++;
			while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0') i++;
		}
		
		if(line[i] == '>' && line[i+1] == '>')
			return false;
	}
	
	int i = pos;
	
	while(line[i] != '\n' && line[i] != '\0'){
		
		if(line[i] == '\"'){
			i++;
			while(line[i] != '\"' && line[i] != '\0') i++;
		}
		
		while(!stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
		
		int insertPos = i;
		
		while(stringContainsChar(" \t\n", line[i]) && line[i] != '\0') i++;
		
		
		if(line[i] != '\n' && line[i] != '\0'){
			stringInsert(line, " >>", insertPos);
			i += 3;
		}
	}
	return true;
}

void replaceQuickPrints(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		
		int pos = strlen(line)-1;
		while(pos >= 0 && stringContainsChar(" \t\n", line[pos])) pos--;
		if(pos != -1 && line[pos] == ';'){
			line[pos] = '\n';
			line[pos+1] = '\0';
		}
		
		exclamationPrintParse(line);
		interrogationPrintParse(line);
		coutPrintParse(line);
		cinPrintParse(line);
		
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
}

bool replaceRawIncludesInner(const char * filename){	
	bool replacedSomething = false;
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return replacedSomething;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	char line2[LINESZ];
	while (fgets (line, LINESZ, input)) {
		string includeName = rawInclude(line);
		if(includeName != ""){
			replacedSomething = true;
			
			//Keep whatever is after #raw "file.ext"
			//Eg:
			//#raw "template.cpy" //Some comment
			int aspCnt = 0;
			int i = 0;
			while(aspCnt < 2 && line[i] != '\0')
				if(line[i++] == '\"')
					aspCnt++;			
			if(line[i] != '\n')
				while(line[i] != '\0')
					fprintf(output, "%c", line[i++]);
			
			//Copies include content to current file
			if(fileExist(includeName.c_str())){
				//treatLineEndings(includeName.c_str());
				FILE * copy = fopen(includeName.c_str(), "r");
				if(copy != NULL){
					while (fgets (line2, LINESZ, copy)){
						fprintf(output, "%s", line2);
					}
					fprintf(output, "\n");
					fclose(copy);
				}
			}
			else {
				printf("Raw include file not found: %s", includeName);
			}
		}
		else
			fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return replacedSomething;
}

void replaceRawIncludes(const char * filename){
	int timeout = 1000;
	while(replaceRawIncludesInner(filename) && timeout--);
	if(timeout <= 0)
		printf("Too many iterations on raw replacement | File: %s\n", filename);
}

void joinContinuedLines(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		for(int i = 0; line[i] != '\0';){
			int j = i+1;
			while(stringContainsChar(" \t", line[j])) j++;
			if(line[i] == '\\' && line[j] == '\n'){
				line[i] = ' ';
				line[j] = ' ';
			}
			i = j;
		}
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return;
	
}

void formatLineSpacing(char * line){
	int resultPos = 0;
	int i = 0;
	while(line[i] == ' ' || line[i] == '\t')
		line[resultPos++] = line[i++];
	
	while(line[i] != '\0'){
		line[resultPos++] = line[i++];
		if(line[i] == ' ' || line[i] == '\t'){
			line[resultPos++] = ' ';
			while(line[i] == ' ' || line[i] == '\t')
				i++;
		}
	}
	if(line[i-1] == ' ')
		line[resultPos-1] = '\0';
	else
		line[resultPos] = '\0';
}

void implyWordSeparationSpaces(char * line){
	if(line[0] == '#')
		return;
	
	string resultString = "";
	for(int i = 0; line[i] != '\0'; i++){
		
		if(line[i] == '\"' && line[i-1] != '\\'){
			resultString += line[i++];
			while(!(line[i] == '\"' && line[i-1] != '\\') && line[i] != '\0')
				resultString += line[i++];
		}
		
		int j = i;
		while(stringContainsChar("+-*/%=<>&^|!?,", line[j]))
			j++;
		
		if(i != j){
			char last;
			char next;
			int k = i-1;
			while((line[k] == ' ' || line[k] == '\t') && k >= 0)
				k--;
			if(k != -1)
				last = line[k];
			else
				last = '\0';
			
			k = j;
			while((line[k] == ' ' || line[k] == '\t' || line[k] == '\n') && line[k] != '\0')
				k++;
			next = line[k];
			
			bool isOperator = !stringContainsChar("+-*/%=<>&^|!?,;.([{", last) && last != '\0' &&
							  !stringContainsChar(")]};,.", next) && next != '\0';
			
			if(	isOperator && 
				(line[i] != '/' || line[i+1] != '/') &&
				(line[i] != '/' || line[i+1] != '*') &&
				(line[i] != '*' || line[i+1] != '/') &&
				(line[i] != '-' || line[i+1] != '>') &&
				line[i] != ',')
				resultString += ' ';
			
			
			k = i;
			while(k < j)
				resultString += line[k++];
			
			if(	isOperator && 
				(line[i] != '/' || line[i+1] != '/') &&
				(line[i] != '/' || line[i+1] != '*') &&
				(line[i] != '*' || line[i+1] != '/') &&
				(line[i] != '-' || line[i+1] != '>'))
				resultString += ' ';
				
			i = k-1;
		}
		else{
			resultString += line[i];
		}
	}
	
	strcpy(line, resultString.c_str());
}

void formatSpacing(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		implyWordSeparationSpaces(line);
		formatLineSpacing(line);
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return;
}

void treatCurlyBrackets(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		stack<int> open;
		for(int i = 0; line[i] != '\0'; i++){
			if(line[i] == '{')
				open.push(i);
			else if(line[i] == '}')
				if(open.empty())
					line[i] = ' ';
				else
					open.pop();
		}
		
		while(!open.empty()){
			line[open.top()] = ' ';
			open.pop();
		}
		
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return;
}

void makeMultipleReturnAssignments(char * input){
	char line[LINESZ];
	strcpy(line, input);
	
	int i = 0;
	while(line[i] != '\0' && line[i] != '=') i++;
	if(line[i] == '\0') return;
	
	//Equity found
	
	int equityPosition = i;
	i++;
	
	while(line[i] == ' ' || line[i] == '\t') i++;
	while(line[i] != '\0' && !stringContainsChar(wordSeparators, line[i])) i++;
	while(line[i] == ' ' || line[i] == '\t') i++;
	
	if(line[i] != '(') return;
	
	//Only one name after assignment assured
	//( found
	
	while(line[i] != '\0' && line[i] != ')') i++;
	if(line[i] == '\0') return;
	i++;
	
	//) found
	
	while(line[i] != '\0'){
		if(stringContainsChar(wordSeparators, line[i])) i++;
		else if(line[i] != '\0') return;
	}
	
	//Nothing after ) assured
	
	string functionCall = string(line + equityPosition);
	line[equityPosition] = '\0';
	
	i = 0;
	string offSet = "";
	while(stringContainsChar(wordSeparators, line[i])) offSet += line[i++];
	
	int j = 0;
	while(line[i] != '\0') line[j++] = line[i++];
	line[j] = '\0';
	
	string result = offSet + "unpackingFunctionArguments " + functionCall;
	
	vector<string> words = smartSplitWords(line, "", ",");
	
	if(words.size() <= 1){
		return;
	}
	
	for(int i = 0; i < words.size(); i++){
		words[i] = removeTrailingWhitespace(words[i]);
		if(words[i] == "_")
			continue;
		result += offSet + words[i] + " = get<" + to_string(i) + ">(unpackingFunctionArguments)\n";
	}

	strcpy(input, result.c_str());
}

void implyMultipleReturnValues(char * line){
	if(getFirstWord(line) != "return")
		return;
	
	int i = 0;
	while(line[i] == ' ' || line[i] == '\t') i++;
	i += 7;
	
	vector<string> words = smartSplitWords(line + i, "", ",");
	
	if(words.size() <= 1)
		return;
	
	string result = "";
	for(int j = 0; j < i; j++)
		result += line[j];
	result += "make_tuple(";
	for(string word : words)
		result += removeTrailingWhitespace(word) + ", ";
	result[result.size()-2] = ')';
	result[result.size()-1] = '\n';
	strcpy(line, result.c_str());	
}

void finalLineReplaces(const char * filename){
	FILE * input = fopen(filename, "r");
	if(input == NULL)
		return;
	
	FILE * output = fopen(tempFile, "w+");
	char line[LINESZ];
	while (fgets (line, LINESZ, input)) {
		implyMultipleReturnValues(line);
		makeMultipleReturnAssignments(line);
		forLoopParse(line);
		rofLoopParse(line);
		fprintf(output, "%s", line);
	}
	fclose(input);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	return;
}

void firstReplaces(const char * filename){
	treatLineEndings(filename);
	replaceRawIncludes(filename);
	joinContinuedLines(filename);
	treatCurlyBrackets(filename);
	replaceQuickPrints(filename);
	formatSpacing(filename);
	finalLineReplaces(filename);
}