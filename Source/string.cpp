#include "string.h"

bool stringContainsChar(const string& s, const char& check){
	return s.find_first_of(check) != std::string::npos;
}

bool stringContainsChars(const string& s1, const string& s2){
	return s1.find(s2) != std::string::npos;
}

bool isWhitespace(const string& s, const string& ignore){
	return stringContainsChars(s, ignore);
}

string removeStartEndWhitespace(const string& s, const string& whitespace){
	auto input = s;
	input.erase(0, input.find_first_not_of(whitespace));
	input.erase(input.find_last_not_of(whitespace) + 1);
	return input;
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

int string_isWord(const char* mainStr, const string& subStr, const string& separators){
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
	std::string strInsert(insert);
	stringInsert(dest, strInsert, index);
}

void stringInsert(char* dest, const string& insert, int index){
	std::string strDest(dest);
	strDest.insert(index, insert);
	strcpy(dest,strDest.c_str());
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
		printf("typedef struct name not properly specified\n%s\n", line);
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
	return string_isSubstring(line, "typedef") >= 0;
}

void stringToLower(char* s){
	for(int i = 0; s[i]; i++)
		s[i] = tolower(s[i]);
}
