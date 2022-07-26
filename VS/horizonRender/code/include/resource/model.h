#ifndef MODEL_GUARD
#define MODEL_GUARD
#include "math/vec.h"
#include "util/globals_render.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
struct texture;
class rmodel;

struct DLLAPI_RENDER vertex{
	vec3 pos;
	vec3 normal;
	vec2 uv;
};

struct DLLAPI_RENDER triangle{
	vertex v[3];
};

struct e_model;

class DLLAPI_RENDER model{
public:
	string name;
	vector<triangle> tris;
	texture* t = 0;
	rmodel* rm = 0;
	void add(model B);
	void recalculateNormals();
	e_model *toEmodel();
	AABB getAABB();
};

#endif