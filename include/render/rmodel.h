#ifndef RMODEL_GUARD
#define RMODEL_GUARD
#include <vector>
using std::vector;
#include "vec.h"
struct model;

class rmodel{
	public:
	vector<vec3> *vertices;
	vector<vec3> *colors;
	vector<vec3> *normals;
	vector<vec2> *uvs;
	rmodel();
	rmodel(model *m);
	~rmodel();
	void finalize();
	void clear();
};

#endif