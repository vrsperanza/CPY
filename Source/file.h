#ifndef _FILE_H_43674_
#define _FILE_H_43674_

bool fileExist (char * fileName);
bool isOverwritable(char * fileName);
void copyFile(const char * source, const char * dest);
bool directoryExists(const char *path);

#endif