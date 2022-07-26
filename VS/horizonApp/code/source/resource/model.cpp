#include "resource/model.h"
#include "generators/editmodel.h"
#include "math/simplemath.h"

void model::add(model B){
	for(unsigned int I = 0; I < B.tris.size(); I++){
		tris.push_back(B.tris[I]);
	}
}

void model::recalculateNormals(){
	for(unsigned int I = 0; I < tris.size(); I++){
		auto &A = tris[I].v[0];
		auto &B = tris[I].v[1];
		auto &C = tris[I].v[2];
		vec3 norm = cross((B.pos-A.pos),normalize(C.pos-A.pos));
		A.normal = norm;
		B.normal = norm;
		C.normal = norm;
	}
}

AABB model::getAABB(){
	AABB aabb;
	aabb = aabb.setStart(tris[0].v[0].pos);
	aabb = aabb.setEnd(tris[0].v[0].pos);
	for(unsigned int I = 0; I < tris.size(); I++){
		for(unsigned int J = 0; J < 3; J++){
			vec3 p = tris[I].v[J].pos;
			aabb = aabb.setStart({min(aabb.start.x,p.x),min(aabb.start.y,p.y),min(aabb.start.z,p.z)});
			aabb = aabb.setEnd({max(aabb.end.x,p.x),max(aabb.end.y,p.y),max(aabb.end.z,p.z)});
		}
	}
	return aabb;
}

e_model *model::toEmodel(){
	e_model *EM = new e_model();
	for(unsigned int I = 0; I < tris.size(); I++){
		e_vertex *A = new e_vertex(tris[I].v[0].pos,EM);
		e_vertex *B = new e_vertex(tris[I].v[1].pos,EM);
		e_vertex *C = new e_vertex(tris[I].v[2].pos,EM);
		vec2 uvA = tris[I].v[0].uv; uvA.y = -uvA.y;
		vec2 uvB = tris[I].v[1].uv; uvB.y = -uvB.y;
		vec2 uvC = tris[I].v[2].uv; uvC.y = -uvC.y;
		vec3 vnA = tris[I].v[0].normal;
		vec3 vnB = tris[I].v[1].normal;
		vec3 vnC = tris[I].v[2].normal;
		e_triangle *T = new e_triangle(A,B,C,EM);
		T->uvs[0] = uvA;
		T->uvs[1] = uvB;
		T->uvs[2] = uvC;
		T->vert_normals[0] = vnA;
		T->vert_normals[1] = vnB;
		T->vert_normals[2] = vnC;
	}
	EM->recalculateNeighbors();
	return EM;
}
