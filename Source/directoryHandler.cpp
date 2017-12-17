#include <iostream>
#include "file.h"
#include "globalHeader.h"
#include "extensionHandler.h"

#include <dirent.h>
#include <vector>
#include <cstring>    
#include <string>
#include <set>
using namespace std;

void makeDirectory(const char * path){
	if(!directoryExists(path)){
		string sysCall = "mkdir ";
		sysCall += path;
		system(sysCall.c_str());
	}
}

void CloneFilesToCompilationDirectory(const std::string& path){
	string compDir = "./";
	compDir += compilationDirectory;
	compDir += "/";
	
	if(path == compDir)
		return;
	
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(path.c_str());
    if (dpdf != NULL){
        while ((epdf = readdir(dpdf)) != NULL){
            if(epdf->d_type==DT_DIR && string(epdf->d_name) != ".." && string(epdf->d_name) != "."){
                CloneFilesToCompilationDirectory(path+epdf->d_name+"/");
            }
            if(epdf->d_type==DT_REG && isCodeFile(epdf->d_name)){
				string src = path+epdf->d_name;
				string temp = compilationDirectory;
				string dest = temp+"//"+epdf->d_name;
				if(fileExist(dest.c_str())){
					if(fileModifiedTime(dest.c_str()) < fileModifiedTime(src.c_str())){
						remove(dest.c_str());
						copyFile(src.c_str(), dest.c_str());
					}
				}
				else
					copyFile(src.c_str(), dest.c_str());
            }
        }
    }
    closedir(dpdf);
}

void prepareDirectory(){
	makeDirectory(compilationDirectory);
	CloneFilesToCompilationDirectory("./");
}

void createExportDirectory(set<string> filesDone, string target){
    DIR *dpdf = opendir("./");
    struct dirent *epdf;
	
	string dirPath = "..";
	dirPath += slash;
	dirPath += resultProjectDirectory;
	
	makeDirectory(dirPath.c_str());
	
	while ((epdf = readdir(dpdf)) != NULL){
		if(epdf->d_type==DT_REG && isCppFile(epdf->d_name)){
			string fileName = removeExtension((string)epdf->d_name);
			if(filesDone.count(fileName)){
				copyFile(epdf->d_name, ((string)dirPath + (string)slash + (string)epdf->d_name).c_str());
			}
		}
	}
	
	FILE * makefile = fopen((dirPath + (string)slash + (string)"Makefile").c_str(), "w+");
	fprintf(makefile, "all:\n");
	fprintf(makefile, "\tg++ -o %s ", target.c_str());
	for(string file : filesDone)
		fprintf(makefile, "%s.cpp ", file.c_str());
	
	fprintf(makefile, "\n\n%s: ", target.c_str());
	for(string file : filesDone)
		fprintf(makefile, "%s.o ", file.c_str());
	fprintf(makefile, "\n\tg++ -o %s ", target.c_str());
	for(string file : filesDone)
		fprintf(makefile, "%s.o ", file.c_str());
	
	for(string file : filesDone)
		fprintf(makefile, "\n%s.o: %s.cpp ", file.c_str(), file.c_str());
	
	fclose(makefile);
	closedir(dpdf);
}

/*
all: 
	g++ -o cpy main.cpp dependenciesMapper.cpp extensionHandler.cpp file.cpp directoryHandler.cpp headerGen.cpp line.cpp rawIncludes.cpp sourceGen.cpp string.cpp

cpy: main.o dependenciesMapper.o extensionHandler.o file.o directoryHandler.o headerGen.o line.o rawIncludes.o sourceGen.o string.o
	g++ -o cpy main.o dependenciesMapper.o extensionHandler.o file.o directoryHandler.o headerGen.o line.o rawIncludes.o sourceGen.o string.o
main.o: main.cpp
dependenciesMapper.o: dependenciesMapper.cpp
extensionHandler.o: extensionHandler.cpp
file.o: file.cpp
directoryHandler.o: directoryHandler.cpp
headerGen.o: headerGen.cpp
line.o: line.cpp
rawIncludes.o: rawIncludes.cpp
sourceGen.o: sourceGen.cpp
string.o: string.cpp
*/