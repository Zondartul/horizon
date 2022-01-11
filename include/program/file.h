#ifndef FILE_GUARD
#define FILE_GUARD
//handles file IO
#include "SDL2/SDL.h"
#include <string>
using std::string;

string fileToString(const char *filepath);
string locateResource(const char *type, const char *name);

#endif