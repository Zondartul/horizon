#ifndef PIPE_GUARD
#define PIPE_GUARD
#include "util/globals_gui.h"
#include <cstdio>
#include <string>
using std::string;

struct pipeInfo {
	string name;
	const char* filename;
};
pipeInfo openPipe(string name);
#endif