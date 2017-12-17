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