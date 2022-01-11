#include "modules/display/display.h"
#include "modules/display/paint.h"
#include "modules/os/os.h"
#include "vec.h"
#include "modules/display/renderqueue.h"
#include "GL/gl.h"

void initDisplay(){
	setViewport(rect(200,200));
}

void go3D(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,512,512);
	glOrtho(0,256,256,0,0,1);
	glMatrixMode(GL_MODELVIEW);
}

void render(){
	clearScreen();
	go3D();
	render_the_renderQueue();
	swapWindow();
}
