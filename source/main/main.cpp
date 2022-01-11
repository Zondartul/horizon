#include "SDL2/SDL.h" 
#include <stdio.h>
#include <iostream>
#include "main/control.h"

int main( int argc, char* args[] ) 
{
	//start up procedures - SDL handles all platform specific stuff
	//init SDL library, turn on all subsystems
	SDL_Init(SDL_INIT_EVERYTHING);
	
	//report SDL version
	SDL_version compiled;
	SDL_version linked;
	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("SDL version at compilation: %d.%d.%d\n",compiled.major, compiled.minor, compiled.patch);
	printf("SDL version at runtime:     %d.%d.%d\n",linked.major, linked.minor, linked.patch);
	
	//create a window and attach an OpenGL context
	//(extern SDL_Window *window)
	window = SDL_CreateWindow("Hoziron", 
						SDL_WINDOWPOS_UNDEFINED, 
						SDL_WINDOWPOS_UNDEFINED, 
						1024,
						640,
						SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	
	//(extern SDL_GLContext glcontext)
	glcontext = SDL_GL_CreateContext(window);
	
	atexit(Cleanup);
	
	//start the game
	OnProgramStart();
	//this is the game's main loop.
	bool quit;
	while(!quit){
		//handle events
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					quit = 1;
				break;
				case SDL_WINDOWEVENT:
					switch(event.window.event){
						case SDL_WINDOWEVENT_RESIZED:
							printf("SDL Window resized: %d x %d\n",event.window.data1, event.window.data2);
							//do nothing?
							//screen = getScreenRect();
							glViewport(0, 0, event.window.data1, event.window.data2);
						break;
						default:
							printf("SDL Window Event: %u\n", event.window.event);
						break;
					}
				break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEMOTION:
				case SDL_MOUSEWHEEL:
					keyThing(event);
				break;
				default:
					printf("SDL Event: %u\n", event.type);
				break;
			}
		}
		//do the rest of gameloop stuff.
		ProgramTick();
	}
	//exit here.
	//everything is already registered with atexit().
	return 0;   
}
// SDL_Window *win;
// SDL_Init();				//initializes the entire SDL library and turns on the requested subsystems
//		SDL_INIT_TIMER, AUDIO, VIDEO, JOYSTICK, HAPTIC, GAMECONTROLLER, EVENTS, EVERYTHING
// SDL_CreateWindow()		//
// SDL_Renderer *ren;
// SDL_CreateRenderer();
// SDL_Delay()
// SDL_DestroyWindow();
// SDL_Quit()				//safely shut down SDL (OS doesn't always clean up). Program can keep running without SDL.
// return 0;