#include <iostream>
#include "file.h"
#include "globalHeader.h"
#include "extensionHandler.h"

#include <dirent.h>
#include <vector>
#include <cstring>    
#include <string>
using namespace std;

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
				if(fileExist(dest.c_str()))
					remove(dest.c_str());
				copyFile(src.c_str(), dest.c_str());
            }
        }
    }
    closedir(dpdf);
}

void prepareFolder(){
	if(!directoryExists(compilationDirectory)){
		string sysCall = "mkdir ";
		sysCall += compilationDirectory;
		system(sysCall.c_str());
	}
	CloneFilesToCompilationDirectory("./");
}