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
	bool rof = false;
	vector<string> words = smartSplitWords(line, "for", " \t\n,;()=", true);
	if(words.size() == 0){
		words = smartSplitWords(line, "rof", " \t\n,;()=", true);
		rof = true;
	}

	if(words.size() == 0)
		return;
	
	
	if(stringContainsChars(line, ";:,><")){
		if(rof){
			words[1] = "for";
			string result = "";
			for(string s : words)
				result += s;
			strcpy(line, result.c_str());
		}
		
		return;
	}
	
	if(isWhitespace(words[words.size()-1], " \t\n,;()")) words.pop_back();
	
	
	string offSet = words[0];
	string i;
	string n;
	string step;
	string start;
	string comparison;
	
	i = words[3];
	if(words.size() < 7){
		if(rof){
			start = words[5];
			n = "0";
		}
		else {
			start = "0";
			n = words[5];
		}
		
		if(rof)
			comparison = " >= ";
		else
			comparison = " < ";
	}
	else{
		start = words[5];
		n = words[7];
	}
	
	if(words.size() > 11){
		step = " " + words[9] + "= " + words[11];
		if(words[9] == "-")
			comparison = " >= ";
		else
			comparison = " < ";
	}
	else if(words.size() > 9){
		step = " += " + words[9];
		comparison = " < ";
	}
	else{
		if(rof){
			step = "--";
			comparison = " >= ";
		}
		else {
			step = "++";
			comparison = " < ";
		}
	}
	
	string result = words[0] + "for(" + i + " = " + start + "; " + i + comparison + n + "; " + i + step + ")\n";
	
	strcpy(line, result.c_str());
	return;
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
		for(int i = 3; i < words.size(); i += 2){
			s += " << " + words[i] + " << \" \"";
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
				printf("Raw include file not found: %s", includeName.c_str());
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

int getLineNumber(const char * ptr, const char * str){
	const char * p = str;
	int line = 1;
	while(p != ptr){
		if(*p == '\n') line++;
		p++;
	}
	return line;
}

// see http://en.cppreference.com/w/cpp/language/string_literal
// input: ptr points to opening '"'
// prefixR"delimiter(raw_characters)delimiter"
//        ^ptr
bool isRawStringLiteral(const char * ptr, const char * str){
	const char * p = ptr;
	if(*p != '"' || p == str || p[-1] != 'R') return false;

	// figure out if we have valid prefix (L,u,U,u8)
	p--; // p now points to 'R'
	while(p >= str && (isalnum(*p) || *p=='_')) p--;
	p++;
	// now p points to start of prefix:
	// u8R"...
	// ^p^ptr-1	   (ptr-1-p) is length of prefix
	if(ptr-1-p > 2) return false; // invalid prefix - too long
	if(ptr-1-p == 2) return *p=='u' && p[1]=='8';
	if(ptr-1-p == 1) return *p=='u' || *p=='U' || *p=='L';
	return true; // ptr-1-p == 0, no prefix
}

// endSeq = )delimiter"
// max length of endSeq is 16+2=18 chars (not counting terminating null)
// input: ptr points to opening '"'
bool getRawStringEndSequence(const char * ptr, char endSeq[20]){
	const char * p = ptr;
	if(*p != '"') return false;
	p++;
	ptr++;
	// now p and ptr point to start of delimiter or '(' if there is no delimiter
	while(*p && !(*p=='('||*p==')'||*p=='\\'||isspace(*p))) p++;
	// now p points to '('
	if(p-ptr > 16) return false; // delimiter too long
	if(*p != '(') return false;

	endSeq[0] = ')';
	memcpy(endSeq+1, ptr, p-ptr); // copy delimiter to endSeq
	endSeq[p-ptr+1] = '"';
	endSeq[p-ptr+2] = 0;
	return true;
}

/*
 removeComments() does not handle line continuations, so it should be called after joinContinuedLines()

 removeComments() correctly handles /* and // inside char/string literals except these cases:

 - #include"..." is treated as normal string with escapes, which is not standard-compliant (see q-char-sequence in the C11 standard http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf);
   #include<...> is not treated as string, so // and /* inside <...> will start a comment

 - unclosed char/string literals in inactive preprocessor blocks (warning in g++, error in cpy):
   #if 0
   "/*
   #endif

 - probably in some other nontrivial interactions of strings and comments with preprocessor

*/
void removeComments(const char * filename){
	FILE * input = fopen(filename, "rb");
	if(input == NULL)
		return;

	// read entire input file into src
	fseek(input, 0, SEEK_END);
	int size = ftell(input); // input file size
	fseek(input, 0, SEEK_SET);
	char* src = (char*)malloc(size+1);
	fread(src, 1, size, input);
	src[size] = 0; // null-terminate src
	fclose(input);

	char* dst = (char*)calloc(size+1,1); // cannot be larger than src
	char* p = src;
	char* q = dst;

	while(*p){
		if(*p == '/'){
			p++; // skip '/'
			if(*p == '/'){ // line comment
				p++; // skip '/'
				while(*p && *p != '\n') p++; // skip everything until eof or eol, don't skip eol
			}
			else if(*p == '*'){ // block comment
				p++; // skip '*'
				char* end = strstr(p, "*/");
				if(end){
					*q++ = ' '; // block comment is replaced with a single space char: "int/**/a" -> "int a", not "inta"
					p = end + 2;
				}
				else{ // unclosed block comment
					printf("%s:%d: error: unclosed block comment\n", filename, getLineNumber(p,src));
					exit(1);
				}
			}
			else
				*q++ = '/'; // not a comment - just copy '/' to dst
		}
		else if(*p == '"' || *p == '\''){ // string or char literal
			if(isRawStringLiteral(p,src)){
				char endSeq[20] = {0};
				if(!getRawStringEndSequence(p,endSeq)){
					printf("%s:%d: error: invalid raw string literal\n", filename, getLineNumber(p,src));
					exit(1);
				}
				char* end = strstr(p, endSeq);
				if(!end){
					printf("%s:%d: error: unclosed raw string literal\n", filename, getLineNumber(p,src));
					exit(1);
				}
				end += strlen(endSeq);
				// copy entire contents of raw string to dst
				memcpy(q, p, end-p);
				q += end-p;
				p = end;
			}
			else{
				char c = *p;
				char* start = p;
				*q++ = *p++; // copy opening quote
				while(*p){
					if(*p == '\\'){ // copy \ and char that follows it
						*q++ = *p++;
						if(!*p) break;
						*q++ = *p++;
						continue;
					}
					else if(*p == c || *p == '\n') break;
					*q++ = *p++;
				}
				if(*p == c)
					*q++ = *p++; // copy closing quote
				else{ // *p == 0 (eof) or *p == '\n' (eol)
					printf("%s:%d: error: unclosed character or string literal\n", filename, getLineNumber(start,src));
					exit(1);
				}
			}
		}
		else
			*q++ = *p++; // copy char to dst and increment src and dst pointers
	}

	FILE * output = fopen(tempFile, "wb");
	fwrite(dst, 1, strlen(dst), output);
	fclose(output);
	remove(filename);
	rename(tempFile, filename);
	free(src);
	free(dst);
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
		words[i] = removeStartEndWhitespace(words[i]);
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
		result += removeStartEndWhitespace(word) + ", ";
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
	removeComments(filename);
	treatCurlyBrackets(filename);
	replaceQuickPrints(filename);
	formatSpacing(filename);
	finalLineReplaces(filename);
}
