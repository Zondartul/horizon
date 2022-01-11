#include "mouse.h"
#include "SDL2/SDL.h"

void setMouseRelativeMode(bool mode){
	if(mode){SDL_SetRelativeMouseMode(SDL_TRUE);}
	else	{SDL_SetRelativeMouseMode(SDL_FALSE);}
}
vec2 getMousePos(){
	int x,y;
	SDL_GetMouseState(&x,&y);
	return vec2(x,y);
}
