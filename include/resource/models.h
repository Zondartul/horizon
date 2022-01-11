#ifndef MODELS_GUARD
#define MODELS_GUARD
//models.h
#include "../resource/textureloader.h"
#include "../math/vectors.h"

//a triangle that refers to it's vertices by index within a container.
struct idtriangle
{
	int v[3];
	int& operator [](int N);
	
	//constructs an empty triangle
	idtriangle();
	
	//constructs a triangle with given indices
	idtriangle(int A, int B, int C);
	
	//constructs a triangle by locating given vertices within a container
	//or creating them if they don't exist
	idtriangle(vec A, vec B, vec C, vector<vec> *points);
	
	//returns the triangle's normal (flat)
	vec getNormal(vector<vec> *points);
	
	//returns the 3d plane on which the triangle lies
	vec getPlane(vector<vec> *points);
};
//a triangle that stores it's vertices as 3d vectors
struct vtriangle{
	vec v[3];
	vec& operator [](int N);
	vtriangle();
	vtriangle(vec A, vec B, vec C);
	vec getNormal();
	vec getPlane();
};

enum renderflags
{
	R_NODRAW		=1,
	R_UNLIT			=2,
	R_TEXTURE		=4,
	R_LIGHTEN		=8,  //transparency: 0 = opaque, LIGHTEN+DARKEN = normal
	R_DARKEN		=16, 
	R_MIRROR		=32, //same texture index = same mirror plane.
	R_DEBUG			=64, //overdraws debug data
	R_MINDFUCK		=128//acid
};

//a triangle that stores UV coordinates for each of it's vertices
//also the index of a texture stored in model's texture list.
struct textriangle
{
	vec2f v[3];
	int texid;
	renderflags flags;
};

struct model{
	vector<vec> vertices;
	vector<idtriangle> mesh;
	vector<textriangle> uvmap;
	vector<texture> textures;
	model();
	void add(model *M);
};

/*
struct model
{
	vtriangle *mesh;			int numtris;
	vtriangle *normals;
	textriangle *texmap;	
	texture *textures;		int numtextures;
	vec min,max;
	char blendmode; //0 = opaque, 1 = alpha test only, 2 = object-ordered blending, 3 = shader blending
	model();
	void applyRenderFlags(int R);
	void expandTriangle(vec point, int tri);
	vec getNormalFlat(int tri);
	vector<int> getAdjacentTris(int tri, int vert);
};
*/
#endif