#ifndef WINDOW_GUARD
#define WINDOW_GUARD
#ifndef NO_SDL
#include "Ext/SDL2/SDL.h"
#endif
#include "math/vec.h"
#include "util/globals_render.h"
#include <map>
#include <string>
using std::map;
using std::string;

//assuming single window for now (unless headless)
//extern int g_width;
//extern int g_height;
vec2 DLLAPI_RENDER getScreenSize(void);

void DLLAPI_RENDER OpenGL_printVersion();
void DLLAPI_RENDER OpenGL_getVersion(const unsigned char **version, const char **profile);
void DLLAPI_RENDER OpenGL_init();
void DLLAPI_RENDER OpenGL_swap();

//creates the main window and calls opengl_init
void DLLAPI_RENDER window_init(int h, int w);

//pumps system messages, ignores everything buit "quit".
void DLLAPI_RENDER sysMessageBlankTick();
//sends messages to inputController
void DLLAPI_RENDER sysMessageTick();

extern map<string,bool> DLLAPI_RENDER g_keyboardState;

struct DLLAPI_RENDER gs_windowKind {
	int g_height;
	int g_width;

	map<char, char> g_keyboardMap = {
		{'`','~'},{'1','!'},{'2','@'},{'3','#'},
		{'4','$'},{'5','%'},{'6','^'},{'7','&'},
		{'8','*'},{'9','('},{'0',')'},{'-','_'},
		{'=','+'},{'\\','|'},{';',':'},{'\'','\"'},
		{',','<'},{'.','>'},{'/','?'},{'q','Q'},
		{'w','W'},{'e','E'},{'r','R'},{'t','T'},
		{'y','Y'},{'u','U'},{'i','I'},{'o','O'},
		{'p','P'},{'[','{'},{']','}'},{'a','A'},
		{'s','S'},{'d','D'},{'f','F'},{'g','G'},
		{'h','H'},{'j','J'},{'k','K'},{'l','L'},
		{'z','Z'},{'x','X'},{'c','C'},{'v','V'},
		{'b','B'},{'n','N'},{'m','M'}
	};

	map<string, bool> g_keyboardState;

#ifndef NO_SDL
	SDL_Window* g_mainWindow;
	SDL_GLContext g_mainContext;
#endif
};

#endif