#ifndef MODELS_GUARD
#define MODELS_GUARD
//models.h
#include "../resource/textureloader.h"
#include "../math/vectors.h"
struct idtriangle
{
	int v[3];
	int& operator [](int N);
	
	idtriangle();
	idtriangle(int A, int B, int C);
	idtriangle(vec A, vec B, vec C, vector<vec> &points);
};

vec getNormal(idtriangle t, vector<vec> &points);

struct vtriangle{
	vec v[3];
	vec& operator [](int N);
	vtriangle();
	vtriangle(vec A, vec B, vec C);
	vec getNormal();
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

struct textriangle
{
	vec2f v[3];
	int texid;
	renderflags flags;
};

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
#endif