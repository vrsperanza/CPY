#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "globalHeader.h"

bool fileExist (char * fileName){
	if(access( fileName, F_OK ) != -1)
		return true;
	return false;
}

bool isOverwritable(char * fileName){
	FILE * f = fopen(fileName, "r");
	if(f == NULL)
		return true;
	
	char buff[LINESZ];
	fgets (buff, LINESZ, f);
	
	fclose(f);
	
	int len = strlen(buff);
	
	char autoTag[] = "AutoTag";
	
	int i, j;
	for(i = len - 8, j = 0; buff[i] != '\n' && buff[i] != '\0'; i++, j++)
		if(buff[i] != autoTag[j])
			return false;
	return true;
}