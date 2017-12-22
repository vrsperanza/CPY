#ifndef _GLOBALHEADER_H_43674_
#define _GLOBALHEADER_H_43674_

#define LINESZ 16384
#define tabSpaces 4

#define compilationDirectory "CompilationSource"
#define resultProjectDirectory "ExportedProject"

#define tempFile "autofile.temp"

#define wordSeparators " \t\r\n()[]{},.;!@#%^&*-=+/:\"\'\\"
#define whiteSpace " \t\n"

extern bool beauty;
extern bool run;
extern bool compile;
extern bool silent;
extern bool exportProject;
extern bool cleanCompilation;

#ifdef _WIN32
#define slash "\\"
#else	
#define slash "/"
#endif

#endif