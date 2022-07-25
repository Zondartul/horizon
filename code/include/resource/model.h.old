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
	vec3 pos;
	vec3 normal;
	vec2 uv;
};
struct triangle{
	vertex v[3];
};
struct e_model;
class model{
public:
	string name;
	vector<triangle> tris;
	texture *t;
	rmodel *rm;
	void add(model B);
	void recalculateNormals();
	e_model *toEmodel();
	AABB getAABB();
};
#endif