#ifndef RENDERQUEUE_GUARD
#define RENDERQUEUE_GUARD
#include <vector>
using std::vector;
#include "vec.h"

struct renderable{
	virtual void render()=0;
};

struct r_point2D:public renderable{
	void render();
	vec2i pos;
	vec3ub color;
};

struct r_line2D:public renderable{
	void render();
	vec2i A,B;
	vec3ub cA, cB;
};

struct r_rect2D:public renderable{
	void render();
	rect area;
	vec3ub color;
};

struct r_rect2D_outline:public renderable{
	void render();
	rect area;
	vec3ub color;
};

extern vector<renderable*> renderQueue;
void render_the_renderQueue();
#endif