#include "resource/cursor.h"
#include "stdio.h"
#include <map>
using std::map;

map<string, cursorHandle*> cursors;

cursorHandle *getCursorHandle(string name){
	if(cursors[name]){return cursors[name];}
	else{
		SDL_Cursor *newCursor;
		if(name == "arrow"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);}
		if(name == "ibeam"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);}
		if(name == "wait"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);}
		if(name == "crosshair"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);}
		if(name == "waitarrow"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);}
		if(name == "sizenwse"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);}
		if(name == "sizewe"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);}
		if(name == "sizens"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);}
		if(name == "sizeall"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);}
		if(name == "no"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);}
		if(name == "hand"){newCursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);}
		if(!newCursor){printf("Error: can't get cursor: %s\n", name.c_str()); return 0;}
		cursorHandle *H = new cursorHandle();
		H->cursor = newCursor;
		cursors[name] = H;
		return H;
	}
}

void setCursor(cursorHandle *H){
	SDL_SetCursor(H->cursor);
}