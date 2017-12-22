# CPY
To create the executable simply execute make on the source folder
To compile cpy files use the created executable targeting the main file of the required project:
	cpy main.cpy -flags

```
CPY is a (Pre)Compiler of .cpy files, those are basically C++ withought redundancy.
	Curly brackets are implied from identation
	Semicolons are implied from line breaks (lines must be marked to continue, not to break)
	Headers are interpreted from the sources and includes
	Can return multiple values from functions
	While still being strongly typed (and just as fast as C++, for it is just a pre-compiler)
		variable types can be implied from equity or manually specified.
	Sequenced style declarations have also been generalised to functions:
		"int a, b" works inside function declarations
Simply execute the compiler followed by your source folder and it will create the executable
```
In any of the given examples, "cpy mainFile.cpy" will create a working executable

Features:

Generates all the necessary headers and compiles resulting project. If you include a header "#include "somecode.h"" of a "somecode.cpy", the header will be generated (as well as the cpp file)

Default compilation is optimized in makefile style, only recompiling modified files, increasing overall compilation speed
With the use of a flag (-ex) generates a c++ project folder, makefile included, ready for sharing

Implies semicolons from endlines,
Implies {} using identation,

Implies variable types from assignment:
```
a = 10
b = a
//Declares a and b as integers
```

Can return multiple values from functions:
```
operations(int a, b)
	sum = a + b
	subtract = a - b
	multiply = a * b
	divide = a / b
	return sum, subtract, multiply, divide
	
main()
	sum, _, mult = operations(20, 10)
	! sum mult

//Produces the output:
30 10

//_ variables mean ignore this return value
//The last variables can be omitted if not necessary
```

Implies function argument types based on previous:
```
int somefunction(string a, b)
	code...
//Declares a and b as strings
```

Can imply parenthesis on if, switch, for and while:
```
	if x > 0
		...
	else if x < 0
		...
	for int i = 0; i < 100; i++
		...
	while x--
		...
	switch x
		case 1:
			...
			break
		...
```

Can imply ">>" and "<<" on cin and cout, switch, for and while:
```
	cin a b
	cout a b endl
```

Four new print functions:
```
	! arg1 arg2 arg3
	//! prints a new line, with each argument separated by spaces
	
	!! arg1 arg2 arg3
	//!! prints each argument separated by spaces
	
	? "prompt" variable variable "prompt" variable 
	//? Reads input given inside variables using cin and prints given strings using cout
	
	?? arg1 arg2 arg3
	//?? is meant for debugging, prints a new debug line in the format:
	//arg1 = ContentOfArg1 | arg2 = ContentOfArg2 | arg3 = ContentOfArg3
```

New precompilation tag: #raw "file"
Includes file's content directly in code before exporting, usefull if you can only send one file instead of a project - intended for  templates at competitive coding.

Can be compiled alongside normal cpp files, so hybrid projects are possible <br />
CPY can also compile pure cpp projects, it will automatically link files and compile them intelligently

Only edits clones of the source files in a separate folder

```
USAGE: cpy SourceCode Flags
Flags:
        -h: Shows this help page
        -b: Use line breaks on itermediate code
        -s: Silent compilation, only prints errors
        -ex: Exports project to a directory containing only c++ source and a Makefile
        -r: Automatically runs compiled code
        -nc: Doesn't compile resulting code
        -o target: Specifies target executable name, when not specified target = a
        -OtherFlags: Redirects flag to underlying compiler (g++)
```
