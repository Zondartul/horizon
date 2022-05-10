#ifndef RENDERLOW_GUARD
#define RENDERLOW_GUARD
#include "globals.h"
#include "render.h"
#include "renderLayer.h"

#include "rmodel.h"





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
#pragma warning(disable: 4251) 
	vector<renderOptions> optionStack;
};

extern DLLAPI renderLowKind *g_renderLow;

#endif





