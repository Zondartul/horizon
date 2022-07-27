#ifndef FILE_GUARD
#define FILE_GUARD
//handles file IO
#ifndef NO_SDL
#include "Ext/SDL2/SDL.h"
#endif

#include "util/globals_render.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

string DLLAPI_RENDER fileToString(const char *filepath);
string DLLAPI_RENDER locateResource(const char *type, const char *name);
bool DLLAPI_RENDER fileReadable(string filename);
string DLLAPI_RENDER getProgramDirectory();
string DLLAPI_RENDER getConfigDirectory();
string DLLAPI_RENDER getDataDirectory();
string DLLAPI_RENDER toCanonicalPath(string dir);
vector<string> DLLAPI_RENDER getDirectoryList(string dir);
string DLLAPI_RENDER getOSdirSeparator();
string DLLAPI_RENDER getOSdirInitiator();

string DLLAPI_RENDER getFileExtension(string filename);
string DLLAPI_RENDER getFilename(string filepath);
string DLLAPI_RENDER getFileNameNoExt(string filename);
bool DLLAPI_RENDER isFolder(string filename);
struct texture;
texture DLLAPI_RENDER *getFileIcon(string filename);
bool DLLAPI_RENDER fileExists(string filename);


#endif