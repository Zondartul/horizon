#pragma once
#include "math/vec.h"
#include "render/render.h"
#include "util/globals_gui.h"
#include <vector>
using std::vector;

void DLLAPI_GUI apply(renderOptions O, renderLayer* L);

void DLLAPI_GUI applyImmediate(renderOptions O);

//void pushRenderOptions();
// 
//void popRenderOptions();

class DLLAPI_GUI renderable {
public:
	vec3 pos = vec3(0, 0, 0);
	vec3 rot = vec3(0, 0, 0);
	vec3 color = vec3(255, 255, 255);
	virtual void upload() = 0;
	virtual void render(renderOptions* options) = 0;
	virtual ~renderable() = default;
};

class DLLAPI_GUI renderableSprite :public renderable {
public:
	texture* t = 0;
	rect R;
	void upload();
	void render(renderOptions* options);
	virtual ~renderableSprite() = default;
};

class DLLAPI_GUI renderableText : public renderable {
public:
	string text;
	font* F = 0;
	rmodel* rm = 0;
	//vec3 pos; /// its in base class
	bool uploaded = false;
	void upload();
	void render(renderOptions* options);
	virtual ~renderableText() = default;
};

class DLLAPI_GUI renderableModel :public renderable {
public:
	rmodel* rm_default = 0;
	rmodel* rm_wireframe = 0;
	texture* t = 0;
	void upload();
	void render(renderOptions* options);
	virtual ~renderableModel() = default;
};

class DLLAPI_GUI renderableMultimodel :public renderable {
public:
	vector<vec3> offsets;
	vector<renderableModel*> parts;
	void upload();
	void render(renderOptions* options);
	virtual ~renderableMultimodel() = default;
};

class DLLAPI_GUI renderablePlane :public renderable {
public:
	vec3 normal;
	float offset;
	rmodel* rm = 0;
	texture* t = 0;
	float texscale = 0;
	void upload();
	void render(renderOptions* options);
	virtual ~renderablePlane() = default;
};

void DLLAPI_GUI renderInit();

void DLLAPI_GUI renderTick();

struct DLLAPI_GUI gs_render_guiKind {
	vector<renderOptions> g_renderOptionsStack;
};
