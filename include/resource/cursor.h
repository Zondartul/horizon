#ifndef CURSOR_GUARD
#define CURSOR_GUARD
#include "SDL2/SDL.h"
#include <string>
using std::string;

struct cursorHandle{
	SDL_Cursor* cursor;
};
cursorHandle *getCursorHandle(string name);
void setCursor(cursorHandle *H);

#endif