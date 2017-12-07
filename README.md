# CPY
To create the executable simply run make on the source folder

CPY is a (Pre)Compiler of .cpy files, it compiles code C++ withought curly brackets, semicolons, headers and some parenthesis
Simply execute the compiler followed by your source folder and it will create the executable

In any of the given examples, "cpy main.cpy" will create a working executable

Features:

Generates all the necessary headers and calls g++ for all the linked files
	If you include a header "#include "somecode.h"" of a "somecode.cpy", the header will be generated (as well as the cpp file), compilation will run and they will be removed (unless you use the -k (keep) flag)

Implies semicolons from endlines,
implies {} using identation,
implies function argument types based on previous:
```
	This:
	int somefunction(int someArgument, someArgument2)
		someCode
	
	Becomes:
	int somefunction(int someArgument, int someArgument2){
		someCode;
	}
	Is compiled and then removed
```
Can imply parenthesis on if, switch and loop:
```
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
```

Can be compiled alongside normal cpp files and has some safety features to block deletion of other source files
Currently it is possible to have some .cpy files and some .cpp, it will create .cpp and .h files from .cpy only if they are not present or procedurally generated (if you choose to keep them with -k), linking doesn't edit files, so it is (untested) possible to compile a complete cpp project running "cpy main.cpp"

```
USAGE: cmp SourceCode Flags
Flags:
        -h: Shows this help page
        -b: Use line breaks on itermediate code
        -k: Keeps intermediate code
        -r: Automatically runs compiled code
        -nc: Doesn't compile resulting code
        -o target: Specifies target executable name, when not specified target = a
        -OtherFlags: Redirects flag to underlying compiler (g++)
```
