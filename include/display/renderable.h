#ifndef RENDERABLE_GUARD
#define RENDERABLE_GUARD
#include "../resource/models.h"
#include "stdint.h"

//renderflags determine how a specific renderable is rendered
//RENDER_DEFAULT:		implementation defined
#define RENDER_DEFAULT			(1<<0)
//RENDER_NONE:			do not render (overrides all other flags)
#define RENDER_NONE				(1<<1)
//RENDER_OVERRIDE:		any child elements should inherit current renderflags
#define RENDER_OVERRIDE			(1<<2)
//RENDER_FACES:			draw the front faces
#define RENDER_FACES			(1<<3)
//RENDER_BACKFACES:		draw the back faces
#define RENDER_BACKFACES		(1<<4)
//RENDER_EDGES:			draw the edges
#define RENDER_EDGES			(1<<5)
//RENDER_VERTICES:		draw the vertices
#define RENDER_VERTICES			(1<<6)
//RENDER_TEXTURES:		apply the texture to the body
#define RENDER_TEXTURES			(1<<7)
//COLOR_DEFAULT:		implementation defined
#define COLOR_DEFAULT			(1<<8)
//COLOR_BLEND:			color is blended between the elements
#define COLOR_BLEND				(1<<9)
//COLOR_UNIFORM:		draw with a single color
#define COLOR_UNIFORM			(1<<10)
//LIGHT_DEFAULT:		implementation defined
#define LIGHT_DEFAULT			(1<<11)
//LIGHT_NONE:			do not apply lighting calculations (overrides LIGHT_ALL)
#define LIGHT_NONE				(1<<12)
//LIGHT_ALL:			apply all lighting calculations
#define LIGHT_ALL				(1<<13)
//TRANSPARENCY_DEFAULT:	implementation defined
#define TRANSPARENCY_DEFAULT	(1<<14)
//TRANSPARENCY_NONE:	no transparency (overrides UNIFORM and TEXTURE)
#define TRANSPARENCY_NONE		(1<<15)
//TRANSPARENCY_UNIFORM:	the object's overal transparency is pre-scaled by the alpha value
#define TRANSPARENCY_UNIFORM	(1<<16)
//TRANSPARENCY_TEXTURE:	the object's texture's alpha channel is used for transparency
#define TRANSPARENCY_TEXTURE	(1<<17)

class renderable{
	public:
	//How many frames should the object be alive and rendered for before being deleted. Set to -1 for infinite lifetime.
	int lifetime;
	uint32_t renderflags;
	//32-bit RGBA color
	color4i color;
	//how big the object is. Interpretation is implementation-defined.
	float thickness;
	//default base constructor
	renderable();
	//override this method with a rendering function specific to each child class.
	//it will be externally called when it is time to draw this object to screen.
	virtual void render();
	//same as above, but assumes a temporary value for renderflags.
	void render2(uint32_t flags);
	//sets the renderflags for this object, optionally also applying them to all children
	virtual void setrenderflags(uint32_t flags, bool recursive);
};

//a renderable 0D point embedded in 3D space. Drawn with a specific thickness in pixels.
//default: RENDER_VERTICES | COLOR_UNIFORM | LIGHT_NONE | TRANSPARENCY_NONE
class point: public renderable{
	public:
	//location of the point in 3D space.
	vec pos;
	//default constructor
	point();
	//constructor that sets the .pos
	point(vec newpos);
	//constructor that sets the .pos and .lifetime
	point(vec newpos, int time);
	//rendering function
	void render();
};

//a renderable 1D line embedded in 3D space.
//default: RENDER_EDGES | COLOR_BLEND | LIGHT_NONE | TRANSPARENCY_NONE
class line: public renderable{
	public:
	//the beginning and the end of the line
	point end1;
	point end2;
	//does this line end at either endpoint?
	enum {LINE_SEGMENT, LINE_RAY, LINE_INFINITE} linetype;
	line();
	line(vec newend1, vec newend2);
	line(vec newend1, vec newend2, int time);
	//finds the intersections of the line with the view frustrum.
	//returns 1 on success.
	//returns 0 on failure (the line does not intersect the edges of the screen)
	bool find_frustum_intersections(vec &endA, vec &endB);
	void render();
	void setrenderflags(uint32_t flags, bool recursive);
};

//a renderable 2D triangle embedded in 3D space.
//default: RENDER_FACES | COLOR_BLEND | LIGHT_NONE | TRANSPARENCY_NONE
class rtriangle:public renderable{
	public:
	point A;
	point B;
	point C;
	rtriangle();
	//defines the triangle from 3 vectors
	rtriangle(vec vA, vec vB, vec vC);
	rtriangle(vec vA, vec vB, vec vC, int time);
	//defines the triangle from an index-triangle and the point-vector to which it refers.
	rtriangle(idtriangle tri, vector<vec> *points, int time);
	void render();
	void setrenderflags(uint32_t flags, bool recursive);
};

//a renderable 2D sprite text embedded in 3D space.
//default: RENDER_DEFAULT | COLOR_UNIFORM | LIGHT_NONE | TRANSPARENCY_NONE
class floatingtext: public renderable{
	public:
	string text;
	//the text will be shifted, on screen, by this much, relative to where the point
	//is when seen from the screen.
	vec2i screen_offset;
	//position of the text origin point in 3D space. 
	vec pos;
	floatingtext();
	floatingtext(vec newpos, string newtext);
	floatingtext(vec newpos, string newtext, int time);
	void render();
};

//a renderable 3D shape embedded in 3D space.
//default: RENDER_FACES | COLOR_BLEND | LIGHT_NONE | TRANSPARENCY_NONE
class rmodel:public renderable{
	public:
	//a cache of rendertriangles
	vector<rtriangle> triangles;
	//pointer to model data
	model *M;
	rmodel(model *m);
	void render();
	//re-create the rendertriangles from the model
	void regenerate();
	void setrenderflags(uint32_t flags, bool recursive);
};
#endif