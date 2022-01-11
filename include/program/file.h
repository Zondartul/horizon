#ifndef FILE_GUARD
#define FILE_GUARD
//handles file IO
#include "SDL2/SDL.h"
#include <string>
using std::string;
#include <vector>
using std::vector;

string fileToString(const char *filepath);
string locateResource(const char *type, const char *name);
string getProgramDirectory();
string getConfigDirectory();
string toCanonicalPath(string dir);
vector<string> getDirectoryList(string dir);

string getFileExtension(string filename);
bool isFolder(string filename);
struct texture;
texture *getFileIcon(string S);

#endif