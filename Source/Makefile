all: cpy.exe

cpy.exe: main.o dependenciesMapper.o extensionHandler.o file.o directoryHandler.o headerGen.o line.o rawIncludes.o sourceGen.o string.o
	g++ -o cpy.exe main.o dependenciesMapper.o extensionHandler.o file.o directoryHandler.o headerGen.o line.o rawIncludes.o sourceGen.o string.o

main.o: main.cpp globalHeader.h sourceGen.cpp headerGen.cpp string.cpp file.cpp line.cpp extensionHandler.cpp dependenciesMapper.cpp rawIncludes.cpp directoryHandler.cpp
	g++ -c main.cpp sourceGen.cpp headerGen.cpp string.cpp file.cpp line.cpp extensionHandler.cpp dependenciesMapper.cpp rawIncludes.cpp directoryHandler.cpp

dependenciesMapper.o: dependenciesMapper.cpp extensionHandler.cpp line.cpp
	g++ -c dependenciesMapper.cpp extensionHandler.cpp line.cpp

extensionHandler.o: extensionHandler.cpp
	g++ -c extensionHandler.cpp

file.o: file.cpp
	g++ -c file.cpp

directoryHandler.o: directoryHandler.cpp file.cpp extensionHandler.cpp
	g++ -c directoryHandler.cpp file.cpp extensionHandler.cpp

headerGen.o: headerGen.cpp main.cpp globalHeader.h sourceGen.cpp string.cpp file.cpp line.cpp extensionHandler.cpp dependenciesMapper.cpp rawIncludes.cpp directoryHandler.cpp
	g++ -c headerGen.cpp main.cpp sourceGen.cpp string.cpp file.cpp line.cpp extensionHandler.cpp dependenciesMapper.cpp rawIncludes.cpp directoryHandler.cpp
 
line.o: line.cpp string.cpp
	g++ -c line.cpp string.cpp

rawIncludes.o: rawIncludes.cpp extensionHandler.cpp file.cpp line.cpp string.cpp sourceGen.cpp
	g++ -c rawIncludes.cpp extensionHandler.cpp file.cpp line.cpp string.cpp sourceGen.cpp

sourceGen.o: sourceGen.cpp main.cpp globalHeader.h headerGen.cpp string.cpp file.cpp line.cpp extensionHandler.cpp dependenciesMapper.cpp rawIncludes.cpp directoryHandler.cpp
	g++ -c sourceGen.cpp main.cpp headerGen.cpp string.cpp file.cpp line.cpp extensionHandler.cpp dependenciesMapper.cpp rawIncludes.cpp directoryHandler.cpp
 
string.o: string.cpp
	g++ -c string.cpp