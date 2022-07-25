#ifndef WINDOW_GUARD
#define WINDOW_GUARD
//#include "vec.h"
//#include <map>
//#include <string>
//using std::map;
//using std::string;

//assuming single window for now (unless headless)
//extern int width;
//extern int height;

//vec2 getScreenSize();
dll_vec2f getScreenSize();

dll_string OpenGL_printVersion();
void OpenGL_getVersion(const unsigned char **version, const char **profile);
void OpenGL_init();
void OpenGL_swap();

//creates the main window and calls opengl_init
void window_init(int h, int w);

//pumps system messages, ignores everything buit "quit".
void sysMessageBlankTick();
//sends messages to inputController
void sysMessageTick();

extern map<string,bool> keyboardState;


#endif