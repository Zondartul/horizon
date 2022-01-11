#ifndef RENDER_GUARD
#define RENDER_GUARD
#include "vec.h"
/*
	How render works:
	call flow:
	
	renderLow			//does low-level OpenGL 2.x or 3.x drawing,
	^					//or maybe even a software rasterizer.
	|
	renderQueue			//queues up render commands from different threads
	^
	|
	renderImmediate		//draws lines, text, triangles for this frame only
	^
	|
	renderScene			//keeps track of objects that are re-rendered until
						//until dismissed.
	data flow:
	
	screen
	^
	|
	renderTarget		//a rendertarget is a handle for a surface onto which
	^					//stuff is rendered, be it a screen or a RAM buffer.
	|
	renderLayer			//a layer creates ordering between different
	^					//render contexts (setting sets)
	|
	renderScene			//storage of objects that should keep being drawn
	
*/

struct rmodel;
struct texture;

struct renderable{
	vec3 color;
	rmodel *rm;
	texture *t;
	void upload();
};

#endif