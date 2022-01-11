#ifndef MODELLOADER_GUARD
#define MODELLOADER_GUARD
#include <string>
using std::string;
#include <vector>
using std::vector;
#include "vec.h"
#include "textureLoader.h"

struct vertex{
	vec3f pos;
	vec3f normal;
	vec2f uv;
};

struct triangle{
	vertex v[3];
};
class rmodel;
struct model{
	vector<triangle> tris;
	texture t;
	rmodel *rm;
	//int handle[4];
	void add(model B);
	void recalculateNormals();
	AABB getAABB();
};

#include <map>
using std::map;
extern map<string,model> modelCache;

model getModel(string name);
model loadModel(const char *filepath);

#endif