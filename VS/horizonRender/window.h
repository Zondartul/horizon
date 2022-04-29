#ifndef WINDOW_GUARD
#define WINDOW_GUARD
#include "vec.h"
#include <map>
#include <string>
using std::map;
using std::string;

//assuming single window for now (unless headless)
//extern int g_width;
//extern int g_height;
vec2 getScreenSize();

void OpenGL_printVersion();
void OpenGL_getVersion(const unsigned char **version, const char **profile);
void OpenGL_init();
void OpenGL_swap();

//creates the main window and calls opengl_init
void window_init(int h, int w);

//pumps system messages, ignores everything buit "quit".
void sysMessageBlankTick();
//sends messages to inputController
void sysMessageTick();

extern map<string,bool> g_keyboardState;

struct gs_windowKind {
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