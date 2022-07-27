#ifndef PIPE_GUARD
#define PIPE_GUARD
#include "util/globals_program.h"
//#include <cstdio>
#include <string>
using std::string;

struct DLLAPI_PROGRAM pipeInfo {
	string name;
	const char* filename;
};
pipeInfo DLLAPI_PROGRAM openPipe(string name);

#endif