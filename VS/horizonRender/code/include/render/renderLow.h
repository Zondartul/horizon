#ifndef RENDERLOW_GUARD
#define RENDERLOW_GUARD
#include "render/render.h"
#include "render/renderLayer.h"
#include "loaders/modelLoader.h"
#include "render/rmodel.h"
#include "util/globals_render.h"
#include "util/event.h"
#include "resource/bitmap.h" /// for bitmap in renderLow_Soft

/// renderLow, opengl 3.x version


class GPUdriver;

class DLLAPI_RENDER renderLowKind: public eventListener {
public:
	renderLowKind();
	virtual ~renderLowKind();

	virtual void renderLowInit() = 0;
	virtual void renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L = 0) = 0;
	virtual void parseCommand(const renderCommand3& rcmd) = 0;
	//virtual void uploadModel(model* m) = 0;  /// uhh these aren't used, right?
	//virtual void uploadModel(rmodel* rm) = 0;
	//virtual void renderModel(model* m) = 0;
	//virtual void unloadModel(rmodel* rm) = 0;
	virtual void setViewportSize(int width, int height) = 0;

	virtual void onEvent(eventKind event) = 0;

	virtual void resetOptions() = 0; ///this, and setviewport, should be bundled into lowInit().
	GPUdriver* gpu;
	renderOptions options;
#pragma warning(disable: 4251) //don't say vector<> needs a dll-interface
	vector<renderOptions> optionStack;
};
extern DLLAPI_RENDER renderLowKind *g_renderLow;

class DLLAPI_RENDER renderLow_SDL : public renderLowKind {
public:
	renderLow_SDL();
	virtual ~renderLow_SDL();

	void renderLowInit();
	void renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L = 0);
	void parseCommand(const renderCommand3& rcmd);
	//void uploadModel(model* m);
	//void uploadModel(rmodel* rm);
	//void renderModel(model* m);
	//void unloadModel(rmodel* rm);
	void setViewportSize(int width, int height);

	void onEvent(eventKind event);

	void resetOptions();
};

/// duplicates commands to two or more other renderLow instances
class DLLAPI_RENDER renderLow_Splitter : public renderLowKind {
public:
	vector<renderLowKind*> children;

	renderLow_Splitter();
	virtual ~renderLow_Splitter();

	void renderLowInit();
	void renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L = 0);
	void parseCommand(const renderCommand3& rcmd);
	void setViewportSize(int width, int height);
	void onEvent(eventKind event);

	void resetOptions();
};

class DLLAPI_RENDER renderLow_Soft : public renderLowKind {
public:
	//bitmap bmp_viewport;
	//bitmap bmp_depth;
	//std::vector<bitmap> textures;
	//std::vector<model> models;
	//std::vector<rmodel> rmodels;

	renderLow_Soft();
	virtual ~renderLow_Soft();

	/// inherited
	void renderLowInit();
	void renderParseQueue(vector<renderCommand3*>* rqueue, renderLayer* L = 0);
	void parseCommand(const renderCommand3& rcmd);
	//void uploadModel(model* m);
	//void uploadModel(rmodel* rm);
	//void renderModel(model* m);
	//void unloadModel(rmodel* rm);
	void setViewportSize(int width, int height);

	void onEvent(eventKind event);

	void resetOptions();
	/// new
	void compose_total_bmp(); /// outputs a single bitmap containing all the data.
	void compose_rqueue_slideshow(); /// outputs a step-by-step animation of how a frame was constructed.
};


struct gs_renderLowKind {
	bool g_debug = false;
};

#endif





