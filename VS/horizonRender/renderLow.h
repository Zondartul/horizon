#ifndef RENDERLOW_GUARD
#define RENDERLOW_GUARD
#include "render.h"
#include "renderLayer.h"
#include "modelLoader.h"
#include "rmodel.h"
#include "globals.h"
//renderLow, opengl 3.x version

struct GPUdriverKind;

class DLLAPI renderLowKind {
public:
	renderLowKind();

	void renderLowInit();
	void renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L = 0);
	void parseCommand(const renderCommand3& rcmd);
	void uploadModel(model* m);
	void uploadModel(rmodel* rm);
	void renderModel(model* m);
	void unloadModel(rmodel* rm);
	void setViewportSize(int width, int height);

	GPUdriverKind* GPUdriver;
	renderOptions options;
#pragma warning(disable: 4251) //don't say vector<> needs a dll-interface
	vector<renderOptions> optionStack;
};
extern DLLAPI renderLowKind *g_renderLow;

struct gs_renderLowKind {
	bool g_debug = false;
};

#endif





