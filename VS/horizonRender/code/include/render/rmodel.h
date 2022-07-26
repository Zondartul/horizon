#ifndef RMODEL_GUARD
#define RMODEL_GUARD
#include "math/vec.h"
#include "util/globals_render.h"
#include <vector>
using std::vector;
class model;

class DLLAPI_RENDER rmodel{
	public:
	vector<vec3> *vertices;
	vector<vec3> *colors;
	vector<vec3> *normals;
	vector<vec2> *uvs;
	rmodel();
	rmodel(model *m);
	~rmodel();
	model *toModel();
	void finalize();
	void clear();
};

struct DLLAPI_RENDER rmpack{
	union{
		rmodel *rm[4];
		struct{
			rmodel *rm_verts;
			rmodel *rm_edges;
			rmodel *rm_tris;
			rmodel *rm_wire;
		};
	};
	rmpack();
};

#endif