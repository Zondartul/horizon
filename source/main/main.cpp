#include "globals.h"
#include "stdio.h"
#include "window.h"
#include "renderLow.h"
#include "SDL2/SDL.h"
#include "modelLoader.h"
#include "fontLoader.h"
#include "inputController.h"
#include "camera.h"
#include "fonts.h"
#include "printw.h"
#include "model.h"
#include "resource.h"
model *mBox;

int main(int argc, char **argv){
	printf("Hello World!\n");
	debuginit();
	window_init(512,512);
	OpenGL_printVersion();
	renderLowInit();
	initFreeType();
	setFont(getFont("calibri 18"));
	
	//renderTest1();
	camera.setPos({-0.5,0,0});
	//renderTest();
	setbuf(stdout,0);
	mBox = getModel("box");
	uploadModel(mBox);
	printf("size of mat4: %d\n",sizeof(mat4));
	printf("size of void*: %d\n",sizeof(void*));
	printf("size of int: %d\n",sizeof(int));
	printf("size of char: %d\n",sizeof(char));
	while(1){
		//renderTest1Tick();
		inputController.think();
		renderTick();
		sysMessageTick();
		//SDL_Delay(1);
	}
	return 0;
}