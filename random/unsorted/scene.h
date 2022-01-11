#ifndef SCENE_GUARD
#define SCENE_GUARD

#include "vec.h"
#include <string>
using std::string;
#include "paint.h"
#include "modelLoader.h"
/*
struct r_point{vec3f A;};
struct r_line{vec3f A, B;};
struct r_triangle{vec3f A, B, C;};
struct r_text{vec3f pos; string text;};

struct renderable{
	bool R3D;
	enum renderable_type{
		R_POINT,
		R_LINE,
		R_TRIANGLE,
		R_RECT,
		R_TEXT,
	} type;
	union{
		r_point point;
		r_line line;
		r_triangle triangle;
		r_text text;
	};
	renderOptions *options;
	int lifetime;
};

//layers:
//3D world
//2D gui
//2D debug
enum sceneLayer{SCENE_3DWORLD,SCENE_3DDEBUG,SCENE_2DGUI,SCENE_2DDEBUG};
*/
enum sceneCommandType{S_COLOR,S_VERTEX,S_DRAWPOINTS,S_DRAWLINES,S_DRAWTRIANGLES,S_DRAWTEXT};
struct sceneCommand{
	sceneCommandType type;
	union{
		vector<vec3ub> *color;
		vector<vec3f> *vertex;
		string *text;	
	};
};

//renderable types:
// immediate:
//  points
//  lines
//  text
//  triangles
// retained:
//  mesh
//
//render options:
// lighting
// texture 
// wireframe
// 
//todo:
// color-graph?
// transform-graph?
//
//
enum renderflags {RF_LIT=1,RF_TEXTURED=2,RF_WIREFRAME=4};



class sceneKind{
	public:
	sceneKind();
	void render();
	// void setIndices(vector<vec3f> vi);
	void setColors(vector<vec3ub> colors);
	void setVertices(vector<vec3f> vv);
	// void setNormals(vector<vec3f> vn);
	// void setUVs(vector<vec2f> vuv);
	// void setTextures(vector<texture> t);
	// void setRenderflags(renderflags RF);
	// void setModel(model *M);
	//setAlpha(uint8_t alpha,int index=0);
	//setThickness(uint8_t thickness);
	//setScissor(rect scissor);
	//cropScissor(rect scissor);
	//clearScissor();
	//setTextMaxX(int maxX);
	//setTextMaxY(int maxY);
	//setTexture(texture t);
	//setFont(font f);
	//setModel(model m);
	void drawPoints();	
	void drawLines();
	void drawTriangles();
	void drawText(string text);
	
	vector<sceneCommand> renderQueue;
	vector<vec3ub> *colors;
	vector<vec3f> *verts;
	renderflags RF;
	void feedVerts();
};	
extern sceneKind scene;
#endif
