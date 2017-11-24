# CPY
To create the executable simply compile the source code, "g++ *.cpp -o cpy" on the source folder gets the job done

CPY is a (Pre)Compiler of .cpy files, it compiles code C/C++ withought curly brackets, semicolons, headers and some parenthesis
Simply execute the compiler followed by your source folder and it will create the executable

In any of the given examples, "cpy main.cpy" will create a working executable

Features:

Generates all the necessary headers and calls g++ for all the linked files
	If you include a header "#include "somecode.h"" of a "somecode.cpy", the header will be generated (as well as the cpp file), compilation will run and they will be removed (unless you use the -k (keep) flag)

Implies semicolons from endlines,
implies {} using identation,
implies function argument types based on previous:
	This:
	int somefunction(int someArgument, someArgument2)
		someCode
	
	Becomes:
	int somefunction(int someArgument, int someArgument2){
		someCode;
	}
	Is compiled and then removed
	
Can imply parenthesis on if, switch and loop:
	This:
	if x > 0
		doSomething
	else if x < 0
		for int i = 0; i < 100; i++
			something
	else
		YetAnothersomething
	
	if(somethingCondition)
		...
	
	Becomes:
	if(x > 0){
		doSomething;
	}
	else if(x < 0){
		for(int i = 0; i < 100; i++){
			something;
		}
	}
	else{
		YetAnothersomething;
	}
	
	if(somethingCondition){
		...
	}
