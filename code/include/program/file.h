#ifndef FILE_GUARD
#define FILE_GUARD
//handles file IO
#ifndef NO_SDL
#include "SDL2/SDL.h"
#endif

#include <string>
using std::string;
#include <vector>
using std::vector;

string fileToString(const char *filepath);
string locateResource(const char *type, const char *name);
bool fileReadable(string filename);
string getProgramDirectory();
string getConfigDirectory();
string getDataDirectory();
string toCanonicalPath(string dir);
vector<string> getDirectoryList(string dir);
string getOSdirSeparator();
string getOSdirInitiator();

string getFileExtension(string filename);
string getFilename(string filepath);
string getFileNameNoExt(string filename);
bool isFolder(string filename);
struct texture;
texture *getFileIcon(string S);

bool fileExists(string filename);

#endif