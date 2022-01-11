#include "selfaware.h"
#include "SDL2/SDL.h"
#include <string>
using std::string;
#include "globals.h"

extern SDL_Window *mainWindow;

vec2i getScreenSize(){
	int h;
	int w;
	SDL_GetWindowSize(mainWindow, &w, &h);
	return {w,h};
}
void setMouseRelativeMode(bool mode){
	if(mode){SDL_SetRelativeMouseMode(SDL_TRUE);}
	else	{SDL_SetRelativeMouseMode(SDL_FALSE);}
}
vec2i getMousePos(){
	vec2i pos;
	SDL_GetMouseState(&pos.x,&pos.y);
	return pos;
}

string locateResource(const char *type, const char *name){
	SDL_ClearError();
	const char *basepath = SDL_GetBasePath();
	if(!basepath){
		const char *err = SDL_GetError();
		if(err){error("locateResource: SDL error: [%s]\n",err);}
		basepath = "./";
	}
	
	string abspath = basepath;
	string resourcepath = "../resource/";
	string T = type;
	if(T == "font"){abspath += string()+resourcepath+"fonts/" + name + ".ttf";}
	if(T == "texture"){abspath += string()+resourcepath+"textures/" + name + ".jpg";}
	if(T == "model"){abspath += string()+resourcepath+"models/" + name + "/" + name + ".obj";}
	
	//printf("locateResource: attempting [%s]\n",abspath.c_str());
	FILE *f = fopen(abspath.c_str(),"rb");
	if(f){
		printf("%s \"%s\" found\n",type,name);
	}else{
		error("locateResource [%s] failed!\n",abspath.c_str());
	}
	fclose(f);
	return abspath;
}