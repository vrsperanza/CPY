#include <iostream>
#include "file.h"
#include "defines.h"
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

void clearDirectory(const char * path){
	DIR *dpdf = opendir(path);
    struct dirent *epdf;
	while ((epdf = readdir(dpdf)) != NULL)
		remove(((string) path + (string)slash + (string)epdf->d_name).c_str());
	closedir(dpdf);
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

void prepareDirectory(bool cleanCompilation){
	makeDirectory(compilationDirectory);
	if(cleanCompilation)
		clearDirectory(compilationDirectory);
	CloneFilesToCompilationDirectory("./");
}

void buildMakefile(string dirPath, set<string> filesDone, string target){
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
	
	buildMakefile(dirPath, filesDone, target);
	closedir(dpdf);
}