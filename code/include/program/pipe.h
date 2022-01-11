#ifndef PIPE_GUARD
#define PIPE_GUARD
#include "globals.h"
#include "stdio.h"

struct pipeInfo {
	string name;
	const char* filename;

};

pipeInfo openPipe(string name);

#endif