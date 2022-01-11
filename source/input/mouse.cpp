#include "mouse.h"
#include "SDL2/SDL.h"

void setMouseRelativeMode(bool mode){
	if(mode){SDL_SetRelativeMouseMode(SDL_TRUE);}
	else	{SDL_SetRelativeMouseMode(SDL_FALSE);}
}
vec2i getMousePos(){
	vec2i pos;
	SDL_GetMouseState(&pos.x,&pos.y);
	return pos;
}
