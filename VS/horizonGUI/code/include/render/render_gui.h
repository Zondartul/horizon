#pragma once
#include "math/vec.h"
#include "render/render.h"
#include <vector>
using std::vector;

void apply(renderOptions O, renderLayer* L);
void applyImmediate(renderOptions O);
void pushRenderOptions();
void popRenderOptions();
class renderable {
public:
	vec3 pos = vec3(0, 0, 0);
	vec3 rot = vec3(0, 0, 0);
	vec3 color = vec3(255, 255, 255);
	virtual void upload() = 0;
	virtual void render(renderOptions* options) = 0;
	virtual ~renderable() = default;
};
class renderableSprite :public renderable {
public:
	texture* t = 0;
	rect R;
	void upload();
	void render(renderOptions* options);
	virtual ~renderableSprite() = default;
};
class renderableModel :public renderable {
public:
	rmodel* rm_default = 0;
	rmodel* rm_wireframe = 0;
	texture* t = 0;
	void upload();
	void render(renderOptions* options);
	virtual ~renderableModel() = default;
};
class renderableMultimodel :public renderable {
public:
	vector<vec3> offsets;
	vector<renderableModel*> parts;
	void upload();
	void render(renderOptions* options);
	virtual ~renderableMultimodel() = default;
};
class renderablePlane :public renderable {
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
void renderInit();
void renderTick();
struct gs_render_guiKind {
	vector<renderOptions> g_renderOptionsStack;
};
