#ifndef RENDER_GUARD
#define RENDER_GUARD
#include "vec.h"
#include <vector>
using std::vector;
#include "fonts.h"

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

class rmodel;
struct texture;
class renderLayer;
struct camprojection;

struct renderOptions{
	bool wireframe = false;
	bool texturing = true;
	bool lighting = true;
	bool coloring = false;
	bool boundingBoxes = false;
	bool uvColoring = false;
	bool normalColoring = false;
	vec3 color = vec3(0,0,0);
	float alpha = 0;
	bool scissoring = false;
	rect scissor = rect(0, 0);
    //----------------------
    bool transparency = true;
    bool depthMask = true;
    bool depthTest = true;
    bool debug = false;
    camprojection *cpj = 0;
    vec3 pos = vec3(0,0,0);
    vec3 rot = vec3(0,0,0);
    vec3 scale = vec3(1,1,1);
    texture *t = 0;
    font *fnt = 0;
    int renderMode = 3;
    vec2 textPos = vec2(0,0);
    float pointSize = 1.f;
    float lineWidth = 1.f;
    vec3 sunPos = vec3(1,1,1);
    vec3 sunLightColor = vec3(1,1,1);
    vec3 ambientLightColor = vec3(0,0,0);
	vec2 texturePos = vec2(0,0);
	vec2 textureScale = vec2(1,1);
    //--------------------
    renderLayer *layer_3d = 0;
    renderLayer *layer_2d = 0;
    //--------------------
};

#endif
