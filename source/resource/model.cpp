#include "model.h"
#include "simplemath.h"

void model::add(model B){
	for(int I = 0; I < B.tris.size(); I++){
		tris.push_back(B.tris[I]);
	}
}

void model::recalculateNormals(){
	for(int I = 0; I < tris.size(); I++){
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
	for(int I = 0; I < tris.size(); I++){
		for(int J = 0; J < 3; J++){
			vec3 p = tris[I].v[J].pos;
			aabb = aabb.setStart({min(aabb.start.x,p.x),min(aabb.start.y,p.y),min(aabb.start.z,p.z)});
			aabb = aabb.setEnd({max(aabb.end.x,p.x),max(aabb.end.y,p.y),max(aabb.end.z,p.z)});
		}
	}
	return aabb;
}
