#include "globalHeader.h"
#include <iostream>
#include <fcntl.h>   
#include <unistd.h>
#include <string.h>
#include <cstdio>
using namespace std;

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

bool fileExist (const char * fileName){
	if(access( fileName, F_OK ) != -1)
		return true;
	return false;
}

void copyFile(const char * source, const char * dest){
    char buf[BUFSIZ];
    size_t size;

    int src = open(source, O_RDONLY, 0);
    int dst = open(dest, O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0644);

    while ((size = read(src, buf, BUFSIZ)) > 0)
        write(dst, buf, size);
	
	close(src);
	close(dst);
}

bool directoryExists(const char *path){
    struct stat info;

    if(stat( path, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}