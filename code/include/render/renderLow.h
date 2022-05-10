#ifndef RENDERLOW_GUARD
#define RENDERLOW_GUARD
#include "globals.h"
#include "render.h"
#include "renderLayer.h"
//#include "modelLoader.h"
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

struct GPUdriverKind;



class DLLAPI renderLowKind{
	public:
	renderLowKind();

	void renderLowInit();
	void renderParseQueue(vector<renderCommand3*> *rqueue, renderLayer *L = 0);
    void parseCommand(const renderCommand3 &rcmd);
	void uploadModel(model *m);
	void uploadModel(rmodel *rm);
	void renderModel(model *m);
	void unloadModel(rmodel *rm);
    void setViewportSize(int width, int height);

	GPUdriverKind *GPUdriver;
	renderOptions options;
#pragma warning(disable: 4251) //don't say vector<> needs a dll-interface
	vector<renderOptions> optionStack;
};

extern DLLAPI renderLowKind *g_renderLow;

#endif





