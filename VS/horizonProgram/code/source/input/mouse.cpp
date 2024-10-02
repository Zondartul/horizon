#ifndef NO_SDL
#include "SDL.h"
#endif
#include "input/mouse.h"
#include "util/global_vars_program.h"
#include <iostream>

void setMouseRelativeMode(bool mode){
#ifndef NO_SDL
	if(mode){SDL_SetRelativeMouseMode(SDL_TRUE);}
	else	{SDL_SetRelativeMouseMode(SDL_FALSE);}
#endif
}
vec2 getMousePos(){
	int x,y;
#ifndef NO_SDL
	SDL_GetMouseState(&x,&y);
	return vec2(x, y);
#else
	return vec2(0, 0);
#endif
}

#ifndef NO_SDL
SDL_SystemCursor toSDL(eSysCursor cursor){
	switch(cursor){
		case CRS_ARROW: 	return SDL_SYSTEM_CURSOR_ARROW;
		case CRS_IBEAM: 	return SDL_SYSTEM_CURSOR_IBEAM;
		case CRS_WAIT:		return SDL_SYSTEM_CURSOR_WAIT;
		case CRS_CROSSHAIR:	return SDL_SYSTEM_CURSOR_CROSSHAIR;
		case CRS_WAITARROW:	return SDL_SYSTEM_CURSOR_WAITARROW;
		case CRS_SIZENWSE:	return SDL_SYSTEM_CURSOR_SIZENWSE;
		case CRS_SIZENESW:	return SDL_SYSTEM_CURSOR_SIZENESW;
		case CRS_SIZEWE:	return SDL_SYSTEM_CURSOR_SIZEWE;
		case CRS_SIZENS:	return SDL_SYSTEM_CURSOR_SIZENS;
		case CRS_SIZEALL:	return SDL_SYSTEM_CURSOR_SIZEALL;
		case CRS_NO:		return SDL_SYSTEM_CURSOR_NO;
		case CRS_HAND:		return SDL_SYSTEM_CURSOR_HAND;
		default: assert(!"unreachable");
	}
}
#endif

void initMouse(){
	Gp->gs_mouse = new gs_mouseKind();
#ifndef NO_SDL
	SDL_ClearError();
	for(unsigned int i = 0; i < SDL_NUM_SYSTEM_CURSORS; i++){
		void *cursor = SDL_CreateSystemCursor((SDL_SystemCursor)i);
		std::cout << "initMouse(): cursor " << i << " = *" << (size_t)cursor << std::endl;
		Gp->gs_mouse->cursors.push_back(cursor);
		if(!cursor){
			const char *err = SDL_GetError();
			std::cerr << "initMouse(): SDL error [" << err << "]" << std::endl;
			SDL_ClearError();
		}
	}
#endif
}

void setCursor(eSysCursor new_cursor){
#ifndef NO_SDL
	SDL_SystemCursor cursor_idx = toSDL(new_cursor);
	void *cursor_sdl = Gp->gs_mouse->cursors.at((unsigned int)cursor_idx);
	std::cout << "set cursor ("<<(unsigned int)new_cursor<<" -> " << (unsigned int)cursor_idx << " -> *" << (size_t)cursor_sdl << ")" << std::endl;
	SDL_SetCursor((SDL_Cursor*)cursor_sdl);
#endif
}

void* createCursor(Cursor curs){
	void *res = 0;
#ifndef NO_SDL
	SDL_ClearError();
	res = SDL_CreateCursor(curs.data, curs.mask, curs.w, curs.h, curs.hot_x, curs.hot_y);
	if(!res){
		const char *err = SDL_GetError();
		std::cerr << "createCursor(): SDL error [" << err << "]" << std::endl;
		SDL_ClearError();
	}
#endif
	return res;
}
