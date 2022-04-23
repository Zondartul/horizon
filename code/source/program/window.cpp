#include "globals.h"
#include "window.h"

#ifndef NO_SDL
#include <SDL2/SDL.h>
#endif

#include "stdio.h"

#ifndef NO_GLEW
#include "GL/glew.h"
#endif

//the "correct way to glew" says we don't use gl.h any more.
#include "stdlib.h"
#include "inputController.h"
#include "GUI.h"
#include "input.h"
#include "renderLow.h"
extern GUIbase *GUI;

int height;
int width;

#ifndef NO_SDL
SDL_Window *mainWindow;
SDL_GLContext mainContext;
#endif

void OpenGL_printVersion(){
	const unsigned char *version;
	const char *profile;
	OpenGL_getVersion(&version, &profile);
	printf("OpenGL version: %s, profile: %s\n",version, profile);
}

void OpenGL_getVersion(const unsigned char **version, const char **profile){
#ifndef NO_GLEW
	* version = 0;

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
#else
	*version = 0;
	*profile = 0;
#endif
}

void OpenGL_init(){
	//one wonders if this actually does what it claims
#ifndef NO_SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
#ifdef WIN32
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); 
 
#endif
#ifndef NO_SDL
    mainContext = SDL_GL_CreateContext(mainWindow);

	SDL_GL_SetSwapInterval(1); // 1 = updates synchronized to vsync, 0 for immediate (-1=?)
#endif
#ifndef NO_GLEW
#endif
}

void OpenGL_swap(){
#ifndef NO_SDL
	SDL_GL_SwapWindow(mainWindow);
#endif
}

void window_init(int h, int w){
#ifndef NO_SDL
	height = h;
	width = w;

	int err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	atexit(SDL_Quit);
	if(err){error("SDL INIT ERROR: [%s]\n", SDL_GetError());}

	mainWindow = SDL_CreateWindow("Hai",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		h,w,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	OpenGL_init();

	OpenGL_printVersion();
	printf("-------- window init done ---------\n");
#endif
}

void sysMessageBlankTick(){
#ifndef NO_SDL
	SDL_Event sdl_event;
	while(SDL_PollEvent(&sdl_event)){
		if(sdl_event.type == SDL_QUIT){
			exit(0);
		}
	}
#endif
}

map<char,char> keyboardMap = {
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

map<string,bool> keyboardState;

eventKind keyboardTranslate(eventKind event){
	if(event.type == EVENT_KEY_DOWN){
		int key = event.keyboard.printchar;
		bool shift = event.keyboard.mod & MOD_SHIFT;
		if(shift){
			if(keyboardMap.count(key)){
				event.keyboard.printchar = keyboardMap[key];
			}
		}
	}
	return event;
}

bool isprintSafe(int key){
	return ((key > 31) && (key < 127));
}

void sysMessageTick(){
#ifndef NO_SDL
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
			keyboardState[event.keyboard.key] = true;
			printf("key down: [%s]\n",event.keyboard.key);
			event.keyboard.mod = MOD_NONE;
			if(sdl_event.key.keysym.mod & KMOD_SHIFT){event.keyboard.mod = event.keyboard.mod | MOD_SHIFT;}
			if(sdl_event.key.keysym.mod & KMOD_CTRL){event.keyboard.mod = event.keyboard.mod | MOD_CTRL;}
			if(sdl_event.key.keysym.mod & KMOD_ALT){event.keyboard.mod = event.keyboard.mod | MOD_ALT;}
			event.keyboard.printchar = event.keyboard.keycode;	//sdl_event.key.keysym.unicode;
			event = keyboardTranslate(event);
			if(!isprintSafe(event.keyboard.keycode)){
				event.keyboard.printchar = 0;
			}
			goto dispatchEvent;
			break;
		case(SDL_KEYUP):
			event.type = EVENT_KEY_UP;
			event.keyboard.keycode = sdl_event.key.keysym.sym;
			event.keyboard.key = SDL_GetKeyName(sdl_event.key.keysym.sym);
			keyboardState[event.keyboard.key] = false;
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
			switch(sdl_event.button.button){
				case(SDL_BUTTON_LEFT):event.mousebutton.button = MOUSE_LEFT;break;
				case(SDL_BUTTON_MIDDLE):event.mousebutton.button = MOUSE_MIDDLE;break;
				case(SDL_BUTTON_RIGHT):event.mousebutton.button = MOUSE_RIGHT;break;
				case(SDL_BUTTON_X1):event.mousebutton.button = MOUSE_X1;break;
				case(SDL_BUTTON_X2):event.mousebutton.button = MOUSE_X2;break;
			}
			goto dispatchEvent;
			break;
		case(SDL_MOUSEBUTTONUP):
			event.type = EVENT_MOUSE_BUTTON_UP;
			switch(sdl_event.button.button){
				case(SDL_BUTTON_LEFT):event.mousebutton.button = MOUSE_LEFT;break;
				case(SDL_BUTTON_MIDDLE):event.mousebutton.button = MOUSE_MIDDLE;break;
				case(SDL_BUTTON_RIGHT):event.mousebutton.button = MOUSE_RIGHT;break;
				case(SDL_BUTTON_X1):event.mousebutton.button = MOUSE_X1;break;
				case(SDL_BUTTON_X2):event.mousebutton.button = MOUSE_X2;break;
			}
			goto dispatchEvent;
			break;
		case(SDL_MOUSEWHEEL):
			event.type = EVENT_MOUSE_WHEEL;
			event.mousewheel.x = sdl_event.wheel.x;
			event.mousewheel.y = sdl_event.wheel.y;
			goto dispatchEvent;
			break;
		case(SDL_WINDOWEVENT):
			switch(sdl_event.window.event){
				case(SDL_WINDOWEVENT_RESIZED):
					width =  sdl_event.window.data1;
					height = sdl_event.window.data2;
					printf("window resized: %d x %d\n",width,height);
                    if(renderLow){renderLow->setViewportSize(width, height);}
					break;
				default:
					break;
			}
			break;
		default:
			//unknown event:
			//ignore.
			break;
		}
		continue;
		dispatchEvent:
		inputChannel->publishEventSequentialMaskable(event);
	}
#endif
}

vec2 getScreenSize(){
#ifndef NO_SDL
	SDL_GetWindowSize(mainWindow, &width, &height);
#endif
	return {width,height};
}

