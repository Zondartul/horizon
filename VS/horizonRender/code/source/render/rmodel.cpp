#include "render/rmodel.h"
#include "render/renderLow.h" //renderLow decides how rendermodel is loaded/unloaded
#include "resource/model.h"
#include <vector>
using std::vector;

rmpack::rmpack(){
	for(int i = 0; i < 4; i++){
		rm[i] = 0;
	}
}

rmodel::rmodel(){
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
		vertices->push_back(I->v[0].pos);
		vertices->push_back(I->v[1].pos);
		vertices->push_back(I->v[2].pos);
		
		colors->push_back(vec3(1,0,0));
		colors->push_back(vec3(0,1,0));
		colors->push_back(vec3(0,0,1));
		
		normals->push_back(I->v[0].normal);
		normals->push_back(I->v[1].normal);
		normals->push_back(I->v[2].normal);
		
		uvs->push_back(vec2(I->v[0].uv.x,-I->v[0].uv.y));
		uvs->push_back(vec2(I->v[1].uv.x,-I->v[1].uv.y));
		uvs->push_back(vec2(I->v[2].uv.x,-I->v[2].uv.y));
	}
}
rmodel::~rmodel(){
	//assume we are being deleted by renderLow, who already unloaded us.
}
//make sure they're all the same size 
model *modelFromPoints(vector<vec3> *vertices,
					vector<vec3> *normals,
					vector<vec2> *uvs){
	model *m = new model;
	triangle t;
	for(unsigned int i = 0; i+2 < vertices->size();i+=3){
		vertex A;
		A.pos = (*vertices)[i];
		A.normal = (*normals)[i];
		A.uv = (*uvs)[i];
		
		vertex B;
		B.pos = (*vertices)[i+1];
		B.normal = (*normals)[i+1];
		B.uv = (*uvs)[i+1];
		
		vertex C;
		C.pos = (*vertices)[i+2];
		C.normal = (*normals)[i+2];
		C.uv = (*uvs)[i+2];
		
		t.v[0] = A;
		t.v[1] = B;
		t.v[2] = C;
		m->tris.push_back(t);
	}
	return m;
}

model *rmodel::toModel(){
	return modelFromPoints(vertices,normals,uvs);
}



void rmodel::finalize(){
	while(colors->size() < vertices->size()){colors->push_back(vec3(1.0f,1.0f,1.0f));}//default color is white
	while(normals->size() < vertices->size()){normals->push_back(vec3(0.0f,0.0f,0.0f));}
	while(uvs->size() < vertices->size()){uvs->push_back(vec2(0.0f,0.0f));}
}

void rmodel::clear(){
	vertices->clear();
	colors->clear();
	normals->clear();
	uvs->clear();
}