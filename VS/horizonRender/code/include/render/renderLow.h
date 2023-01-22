#ifndef RENDERLOW_GUARD
#define RENDERLOW_GUARD
#include "render/render.h"
#include "render/renderLayer.h"
#include "loaders/modelLoader.h"
#include "render/rmodel.h"
#include "util/globals_render.h"
#include "util/event.h"
//renderLow, opengl 3.x version

struct GPUdriverKind;

class DLLAPI_RENDER renderLowKind: public eventListener {
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

	void onEvent(eventKind event);

	GPUdriverKind* GPUdriver;
	renderOptions options;
#pragma warning(disable: 4251) //don't say vector<> needs a dll-interface
	vector<renderOptions> optionStack;
};
extern DLLAPI_RENDER renderLowKind *g_renderLow;

struct gs_renderLowKind {
	bool g_debug = false;
};

#endif





