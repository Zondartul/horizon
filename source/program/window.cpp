#include "globals.h"
#include "window.h"
#include "SDL2/SDL.h"
#include "stdio.h"
#include "GL/gl.h"
#include "stdlib.h"
#include "inputController.h"

int height;
int width;

SDL_Window *mainWindow;
SDL_GLContext mainContext;

void OpenGL_printVersion(){
	const unsigned char *version;
	const char *profile;
	OpenGL_getVersion(&version, &profile);
	printf("OpenGL version: %s, profile: %s\n",version, profile);
}

void OpenGL_getVersion(const unsigned char **version, const char **profile){
	*version = glGetString(GL_VERSION);
	
	*profile = 0;
	int profileID;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profileID);
	switch(profileID){
		case(SDL_GL_CONTEXT_PROFILE_CORE):
			*profile = "CORE";
			break;
		case(SDL_GL_CONTEXT_PROFILE_COMPATIBILITY):
			*profile = "COMPATIBILITY";
			break;
		case(SDL_GL_CONTEXT_PROFILE_ES):
			*profile = "ES";
			break;
	}
}

void OpenGL_init(){
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); 
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2); 
	//one wonders if this actually does what it claims
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	mainContext = SDL_GL_CreateContext(mainWindow);
		
	SDL_GL_SetSwapInterval(1); // 1 = updates synchronized to vsync, 0 for immediate (-1=?)
	
	glClearColor(0.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGL_swap(){
	SDL_GL_SwapWindow(mainWindow);
}

void window_init(int h, int w){
	height = h;
	width = w;
	
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	
	mainWindow = SDL_CreateWindow("Hai",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		h,w,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		
	OpenGL_init();
}

void sysMessageBlankTick(){
	SDL_Event sdl_event;
	while(SDL_PollEvent(&sdl_event)){
		if(sdl_event.type == SDL_QUIT){
			exit(0);
		}
	}
}


void sysMessageTick(){
	SDL_Event sdl_event;
	while(SDL_PollEvent(&sdl_event)){
		eventKind event;
		switch(sdl_event.type){
		case(SDL_QUIT):
			exit(0);
			break;
		case(SDL_KEYDOWN):
			event.type = EVENT_KEY_DOWN;
			event.keyboard.keycode = sdl_event.key.keysym.sym;
			event.keyboard.key = SDL_GetKeyName(sdl_event.key.keysym.sym);
			goto dispatchEvent;
			break;
		case(SDL_KEYUP):
			event.type = EVENT_KEY_UP;
			event.keyboard.keycode = sdl_event.key.keysym.sym;
			event.keyboard.key = SDL_GetKeyName(sdl_event.key.keysym.sym);
			goto dispatchEvent;
			break;
		case(SDL_MOUSEMOTION):
			event.type = EVENT_MOUSE_MOVE;
			event.mousemove.diff = {sdl_event.motion.xrel,sdl_event.motion.yrel};
			event.mousemove.pos = {sdl_event.motion.x,sdl_event.motion.y};
			goto dispatchEvent;
			break;
		case(SDL_MOUSEBUTTONDOWN):
			event.type = EVENT_MOUSE_BUTTON_DOWN;
			event.mousebutton.button = sdl_event.button.button;
			goto dispatchEvent;
			break;
		case(SDL_MOUSEBUTTONUP):
			event.type = EVENT_MOUSE_BUTTON_UP;
			event.mousebutton.button = sdl_event.button.button;
			goto dispatchEvent;
			break;
		case(SDL_MOUSEWHEEL):
			event.type = EVENT_MOUSE_WHEEL;
			event.mousewheel.x = sdl_event.wheel.x;
			event.mousewheel.y = sdl_event.wheel.y;
			goto dispatchEvent;
			break;
		default:
			//unknown event:
			//ignore.
			break;
		}
		continue;
		dispatchEvent:
		inputController.onEvent(event);
	}
}

vec2i getScreenSize(){
	int h;
	int w;
	SDL_GetWindowSize(mainWindow, &w, &h);
	return {w,h};
}

