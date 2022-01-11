#ifndef RENDERABLE_GUARD
#define RENDERABLE_GUARD
#include "../resource/models.h"
class renderable{
	public:
	int lifetime;
	renderable();
	virtual void render();
};

class point: public renderable{
	public:
	vec pos;
	vec3i color;
	float thickness;
	point();
	point(vec newpos);
	point(vec newpos, int time);
	void render();
};

class line: public renderable{
	public:
	vec end1;
	vec end2;
	float thickness;
	bool infinite;
	vec3i color1;
	vec3i color2;
	line();
	line(vec newend1, vec newend2);
	line(vec newend1, vec newend2, int time);
	bool find_frustum_intersections(vec &endA, vec &endB);
	void render();
};

class rtriangle:public renderable{
	public:
	vec A;
	vec3i colA;
	vec B;
	vec3i colB;
	vec C;
	vec3i colC;
	int type;
	rtriangle();
	rtriangle(vec vA, vec vB, vec vC);
	rtriangle(vec vA, vec vB, vec vC, int time);
	rtriangle(idtriangle tri, vector<vec> points, int time);
	void render();
};

class floatingtext: public renderable{
	public:
	string text;
	vec2i screen_offset;
	vec pos;
	floatingtext();
	floatingtext(vec newpos, string newtext);
	floatingtext(vec newpos, string newtext, int time);
	void render();
};
#endif