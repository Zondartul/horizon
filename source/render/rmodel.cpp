#include "rmodel.h"
#include "renderLow.h" //renderLow decides how rendermodel is loaded/unloaded

rmodel::rmodel(){
	handle[0] = 0;
	handle[1] = 0;
	handle[2] = 0;
	handle[3] = 0;
	vertices = new vector<vec3>;
	colors = new vector<vec3>;
	normals = new vector<vec3>;
	uvs = new vector<vec2>;
}
rmodel::rmodel(model *m){
	vertices = new vector<vec3>;
	colors = new vector<vec3>;
	normals = new vector<vec3>;
	uvs = new vector<vec2>;
	
	for(auto I = m->tris.begin(); I != m->tris.end(); I++){
		vertices->push_back(toVec3(I->v[0].pos));
		vertices->push_back(toVec3(I->v[1].pos));
		vertices->push_back(toVec3(I->v[2].pos));
		
		colors->push_back(vec3(1,0,0));
		colors->push_back(vec3(0,1,0));
		colors->push_back(vec3(0,0,1));
		
		//colors->push_back(vec3(1,1,1));
		//colors->push_back(vec3(1,1,1));
		//colors->push_back(vec3(1,1,1));
		
		normals->push_back(toVec3(I->v[0].normal));
		normals->push_back(toVec3(I->v[1].normal));
		normals->push_back(toVec3(I->v[2].normal));
		
		uvs->push_back(vec2(I->v[0].uv.x,-I->v[0].uv.y));
		uvs->push_back(vec2(I->v[1].uv.x,-I->v[1].uv.y));
		uvs->push_back(vec2(I->v[2].uv.x,-I->v[2].uv.y));
	}
}
rmodel::~rmodel(){
	//can't delete buffers immediately, cause this might not have been
	//drawn yet. so tell the queue to do it.
	if(handle[0]){
		unloadModel(this);
	}
	//also can't delete the CPU-side vertex vectors, same reason.
	//make rmodel sharedly owned or pass rmodel to queue so queue can delete it.
	if(vertices){
		rmodel *rm = new rmodel();
		rm->vertices = vertices;
		rm->colors = colors;
		rm->normals = normals;
		rm->uvs = uvs;
		renderCmd(RCMD::RMODEL_DELETE,p=(void*)rm);//lolhax
	}
}
//make sure they're all the same size 
model *modelFromPoints(vector<vec3> *vertices,
					vector<vec3> *normals,
					vector<vec2> *uvs){
	model *m = new model;
	triangle t;
	for(int i = 0; i+2 < vertices->size();i+=3){
		vertex A;
		A.pos = toVec3f((*vertices)[i]);
		A.normal = toVec3f((*normals)[i]);
		A.uv = toVec2f((*uvs)[i]);
		
		vertex B;
		B.pos = toVec3f((*vertices)[i+1]);
		B.normal = toVec3f((*normals)[i+1]);
		B.uv = toVec2f((*uvs)[i+1]);
		
		vertex C;
		C.pos = toVec3f((*vertices)[i+2]);
		C.normal = toVec3f((*normals)[i+2]);
		C.uv = toVec2f((*uvs)[i+2]);
		
		t.v[0] = A;
		t.v[1] = B;
		t.v[2] = C;
		m->tris.push_back(t);
	}
	return m;
}