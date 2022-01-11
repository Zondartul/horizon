#ifndef RENDERLOW_GUARD
#define RENDERLOW_GUARD
#include "render.h"
#include "renderLayer.h"
#include "modelLoader.h"
#include "rmodel.h"
#include "globals.h"
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

struct GPUdriverKind;

class DLLAPI renderLowKind {
public:
	//renderQueue *currentQueue;
	renderLowKind();

	void renderLowInit();
	//void renderTick();
	//void renderParseQueue(vector<renderCommand2*> *rqueue);
	void renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L = 0);
	void parseCommand(const renderCommand3& rcmd);
	void uploadModel(model* m);
	void uploadModel(rmodel* rm);
	void renderModel(model* m);
	void unloadModel(rmodel* rm);
	void setViewportSize(int width, int height);

	GPUdriverKind* GPUdriver;
	renderOptions options;
	vector<renderOptions> optionStack;
};
extern DLLAPI renderLowKind *renderLow;

//void renderTest1();
//void renderTest1Tick();
//void renderTest();
//void renderParseCommandBuffer(renderCommandBuffer *cmdbuff);
//void renderModel(RCMD command, rmodel *rm);
//extern renderCommandBuffer cmdbuff;
//#define renderCmd(t,d) { renderCommand cmd; cmd.type = t; cmd.d; cmdbuff.push_back(cmd); }
//extern renderQueue *rqueue;

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





