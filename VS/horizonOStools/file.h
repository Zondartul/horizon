#ifndef FILE_GUARD
#define FILE_GUARD
//handles file IO
//#ifndef NO_SDL
//#include "SDL2/SDL.h"
//#endif

//#include <string>
//using std::string;
//#include <vector>
//using std::vector;

#include "dll_io.h"

//first, figure out which of these are actually needed

/*string*/ dll_string fileToString(dll_string filepath);

/*bool*/ int fileReadable(dll_string filename);
/*string*/ dll_string getProgramDirectory();
/*string*/ dll_string getConfigDirectory();
/*string*/ dll_string getDataDirectory();
/*vector<string>*/ dll_v_string getDirectoryList(dll_string dir);
/*string*/ dll_string getOSdirSeparator();
/*string*/ dll_string getOSdirInitiator();

/*string*/ dll_string getFileExtension(dll_string filename);
/*string*/ dll_string getFilename(dll_string filepath);
/*string*/ dll_string getFileNameNoExt(dll_string filename);
/*bool*/ int isFolder(dll_string filename);
/*bool*/ int fileExists(dll_string filename);

//move this into the engine
//struct texture;
//texture *getFileIcon(string S);
///*string*/ cpp_string locateResource(const char *type, const char *name);


#endif