#ifndef WINDOW_GUARD
#define WINDOW_GUARD
#ifdef USE_SDL
	#include "Ext/SDL2/SDL.h"
#endif
#include "math/vec.h"
#include "util/globals_program.h"
#include "util/event.h" //for onEvent to catch resizes
#include <map>
#include <string>
using std::map;
using std::string;

//assuming single window for now (unless headless)
//extern int g_width;
//extern int g_height;
vec2 DLLAPI_PROGRAM getScreenSize(void);

void DLLAPI_PROGRAM OpenGL_printVersion();
void DLLAPI_PROGRAM OpenGL_getVersion(const unsigned char **version, const char **profile);
void DLLAPI_PROGRAM OpenGL_init();
void DLLAPI_PROGRAM OpenGL_swap();

//creates the main window and calls opengl_init
void DLLAPI_PROGRAM window_init(int h, int w);

//pumps system messages, ignores everything buit "quit".
void DLLAPI_PROGRAM sysMessageBlankTick();
//sends messages to inputController
void DLLAPI_PROGRAM sysMessageTick();

extern map<string,bool> DLLAPI_PROGRAM g_keyboardState;

struct DLLAPI_PROGRAM gs_windowKind:public eventListener {
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

#ifdef USE_SDL
	SDL_Window* g_mainWindow;
	SDL_GLContext g_mainContext;
#endif

	void onEvent(eventKind event);
};

void DLLAPI_PROGRAM ProgramResizeViewport(unsigned int width, unsigned int height);

#endif