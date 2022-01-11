#ifndef RENDERLOW_GUARD
#define RENDERLOW_GUARD
#include "renderQueue.h"
#include "modelLoader.h"
#include "rmodel.h"
//renderLow, opengl 3.x version

/*
implicit state:
	coloring on/off
	transparency on/off
	texturing on/off
	lighting on/off, vector, color, ambient
	line_thickness
	scissor on/off, rect
	culling on/off, direction
	
data: (also part of state)
	vertices
	color/alphas
	UVs
	projection
	texture
	
draw commands:
	point
	line
	triangle
	
resource:
	texture upload
	
future stuff:
	polygon
	blendtexture/multitexture
	multiple lights
*/



void renderLowInit();
//void renderTest1();
//void renderTest1Tick();
void renderTick();
//void renderTest();
void renderParseCommandBuffer(renderCommandBuffer *cmdbuff);
void uploadModel(model *m);
void uploadModel(rmodel *rm);
void renderModel(model *m);
void renderModel(RCMD command, rmodel *rm);
void unloadModel(rmodel *rm);
extern renderCommandBuffer cmdbuff;
#define renderCmd(t,d) { renderCommand cmd; cmd.type = t; cmd.d; cmdbuff.push_back(cmd); }


/*
void renderLowInit();
void projectionToCamera();
void unloadModel(rmodel *rm);
void renderTriangle();
void uploadModel(model *m);
void uploadModel(rmodel *rm);
void renderModel(rmodel *rm);
void renderModel(model *m);
void renderTick();
void renderParseCommandBuffer(renderCommandBuffer *cmdbuff);
GLuint uploadShader(GLuint shaderType, const char *filepath);
GLuint uploadShaderProgram(const char *filepath_vertex, const char *filepath_fragment);
*/

#endif





