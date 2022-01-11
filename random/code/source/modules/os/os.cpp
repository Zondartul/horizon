#include "modules/os/os.h"
#include "SDL.h"
#include "stdlib.h"
#include "stdio.h"

SDL_Window *mainWindow;

void openWindow(){
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	SDL_GLContext mainContext;
	
	mainWindow = SDL_CreateWindow("Hai",
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		512, 512, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	mainContext = SDL_GL_CreateContext(mainWindow);
	SDL_GL_SetSwapInterval(1);
	
	//glClearColor(0.0,1.0,1.0,1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//const unsigned char *str = glGetString(GL_VERSION);
	//printf("\nOpenGL version: [%s]\n",str);
	
	int profile;
	const char *profileStr = "???";
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);
	switch(profile){
		case(SDL_GL_CONTEXT_PROFILE_CORE):
			profileStr = "CORE";
			break;
		case(SDL_GL_CONTEXT_PROFILE_COMPATIBILITY):
			profileStr = "COMPATIBILITY";
			break;
		case(SDL_GL_CONTEXT_PROFILE_ES):
			profileStr = "ES";
			break;
	}
	printf("OpenGL profile: %s (%d)\n",profileStr, profile);
}

void sleep(){
	SDL_Delay(15);
}

void osCheckMessages(){
	SDL_Event sdl_event;
	while(SDL_PollEvent(&sdl_event)){
		switch(sdl_event.type){
		case(SDL_QUIT):
			exit(0);
			break;
		}
	}
}

void swapWindow(){
	SDL_GL_SwapWindow(mainWindow);
}