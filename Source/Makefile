all: 
	g++ -std=c++11 -o cpy *.cpp

cpy: main.o dependenciesMapper.o extensionHandler.o file.o directoryHandler.o headerGen.o line.o firstReplaces.o sourceGen.o string.o
	g++ -std=c++11 -o cpy main.o dependenciesMapper.o extensionHandler.o file.o directoryHandler.o headerGen.o line.o firstReplaces.o sourceGen.o string.o
main.o: main.cpp
dependenciesMapper.o: dependenciesMapper.cpp
extensionHandler.o: extensionHandler.cpp
file.o: file.cpp
directoryHandler.o: directoryHandler.cpp
headerGen.o: headerGen.cpp
line.o: line.cpp
firstReplaces.o: firstReplaces.cpp
sourceGen.o: sourceGen.cpp
string.o: string.cpp