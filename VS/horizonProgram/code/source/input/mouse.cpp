#ifndef NO_SDL
#include "Ext/SDL2/SDL.h"
#endif
#include "input/mouse.h"

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
