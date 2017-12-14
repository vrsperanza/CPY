# CPY
To create the executable simply execute make on the source folder
To compile cpy files use the created executable targeting the main file of the required project:
	cpy main.cpy -flags

```
CPY is a (Pre)Compiler of .cpy files, those are basically C++ withought redundancy.
	Curly brackets are implied from identation;
	Semicolons are implied from line breaks (lines must be marked to continue, not to break)
	Headers are interpreted from the sources and includes
	While still being strongly typed (and just as fast as C++, for it is just a pre-compiler)
		variable types can be implied from equity or manually specified.
	Sequenced style declarations have also been generalised to functions:
		"int a, b" works inside function declarations
Simply execute the compiler followed by your source folder and it will create the executable
```
In any of the given examples, "cpy mainFile.cpy" will create a working executable

Features:

Generates all the necessary headers and calls g++ for all the linked files
	If you include a header "#include "somecode.h"" of a "somecode.cpy", the header will be generated (as well as the cpp file), compilation will run and they will be removed (unless you use the -k (keep) flag)

Implies semicolons from endlines,
Implies {} using identation,

Implies variable types from assignment:
```
a = 10
b = a
```
Declares a and b as integers

Implies function argument types based on previous:
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

Can imply parenthesis on if, switch, for and while:
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

New precompilation tag: #raw "file"
Includes file's content directly in code before calling g++, usefull if you can only send one file instead of a big project - intended for competitive coding.

Can be compiled alongside normal cpp files and has some safety features to block deletion of other source files
Currently it is possible to have some .cpy files and some .cpp, it will create .cpp and .h files from .cpy only if they are not present or procedurally generated (if you choose to keep them with -k), linking doesn't edit files, so it is (untested) possible to compile a complete cpp project running "cpy main.cpp"

```
USAGE: cpy SourceCode Flags
Flags:
        -h: Shows this help page
        -b: Use line breaks on itermediate code
        -k: Keeps intermediate code
        -r: Automatically runs compiled code
        -nc: Doesn't compile resulting code
        -nh: Doesn't create headers
        -o target: Specifies target executable name, when not specified target = a
        -OtherFlags: Redirects flag to underlying compiler (g++)
```
