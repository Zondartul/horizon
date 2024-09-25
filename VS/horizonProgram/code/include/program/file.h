#ifndef FILE_GUARD
#define FILE_GUARD
//handles file IO
//#ifdef USE_SDL
//	#include "Ext/SDL2/SDL.h"
//#endif

#include "util/globals_program.h"
#include "util/res_or_err.hpp"
#include "util/zerror.hpp"
#include <vector>
#include <string>
using std::string;
using std::vector;

string DLLAPI_PROGRAM fileToString(const char *filepath);
Result<string,zError> DLLAPI_PROGRAM locateResource(const char *type, const char *name);
bool DLLAPI_PROGRAM fileReadable(string filename);
string DLLAPI_PROGRAM getProgramDirectory();
string DLLAPI_PROGRAM getConfigDirectory();
string DLLAPI_PROGRAM getDataDirectory();
string DLLAPI_PROGRAM toCanonicalPath(string dir);
vector<string> DLLAPI_PROGRAM getDirectoryList(string dir);
string DLLAPI_PROGRAM getOSdirSeparator();
string DLLAPI_PROGRAM getOSdirInitiator();

string DLLAPI_PROGRAM getFileExtension(string filename);
string DLLAPI_PROGRAM getFilename(string filepath);
string DLLAPI_PROGRAM getFileNameNoExt(string filename);
bool DLLAPI_PROGRAM isFolder(string filename);
//struct texture;
//texture DLLAPI_PROGRAM *getFileIcon(string filename);
bool DLLAPI_PROGRAM fileExists(string filename);


#endif