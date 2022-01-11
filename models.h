//models.h

struct triangle
{
	vector v[3];
};

enum renderflags
{
	R_NODRAW		=1,
	R_UNLIT			=2,
	R_TEXTURE		=4,
	R_TRANSPARENT	=8,
	R_MIRROR		=16, //same texture index = same mirror plane.
	R_LENS			=32,
	R_DEBUG			=64, //overdraws debug data
	R_MINDFUCK		=128//acid
};

struct textriangle
{
	vec2i v[3];
	int texid;
	renderflags flags;
	textriangle()
	{
		v[0]=v[1]=v[2] = {0,0};
	}
};

struct texture
{
	int texturetype; //normal, animated, dynamic, etc.
	char* texname;
	void *glTexbind; //or something
};

struct model
{
	triangle *mesh;			int numtris;
	triangle *normals;
	textriangle *texmap;	
	texture *textures;		int numtextures;
	vector min,max;
	model()
	{
		numtris = numtextures = 0;
		mesh = NULL;
		normals = NULL;
		texmap = NULL;
		textures = NULL;
		min = max = {0,0,0};
	}
};