#include "program/pipe.h"
#ifdef WIN32
	#include <windows.h>
#endif
#include <sstream>
#include <exception>
using std::stringstream;
using std::runtime_error;

pipeInfo openPipe(string name) {
	throw runtime_error("openPipe unimplemented");
	//stringstream ss;
	//ss << "\\\\.\\pipe\\" << name;
	//LPCSTR lpName = ss.str().c_str();
	//pipeInfo pi;
	//return pi;
}