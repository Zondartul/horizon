#ifndef RMODEL_GUARD
#define RMODEL_GUARD
#include "modelLoader.h"

class rmodel{
	public:
	vector<vec3> *vertices;
	vector<vec3> *colors;
	vector<vec3> *normals;
	vector<vec2> *uvs;
	int handle[4];
	rmodel();
	rmodel(model *m);
	~rmodel();
};

#endif