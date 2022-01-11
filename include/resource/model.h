#ifndef MODEL_GUARD
#define MODEL_GUARD
#include "vec.h"
#include <vector>
using std::vector;
#include <string>
using std::string;
struct texture;
class rmodel;

struct vertex{
	vec3f pos;
	vec3f normal;
	vec2f uv;
};

struct triangle{
	vertex v[3];
};


struct model{
	string name;
	vector<triangle> tris;
	texture *t;
	rmodel *rm;
	//int handle[4];
	void add(model B);
	void recalculateNormals();
	AABB getAABB();
};

#endif