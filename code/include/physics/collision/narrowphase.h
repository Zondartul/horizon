#ifndef NARROWPHASE_GUARD
#define NARROWPHASE_GUARD
#include "collision.h"
#include <string>
using std::string;

collisioninfo *checkCollisionAABB_AABB(collisionbodyAABB *body1, collisionbodyAABB *body2);
collisioninfo *checkCollisionRay_AABB(collisionbodyRay *bodyRay, collisionbodyAABB *bodyAABB);
collisioninfo *collisionCheckDispatch(collisionbody *body1, collisionbody *body2);
void pairwiseCollisionCheck(collisionbody *body1, collisionbody *body2, collisionOptions &options);

struct gs_narrowphaseKind {
	int g_numCollisionPairs = 0;//entity *I, entity *J,

	string g_sideNames[6] = {
		"right",
		"left",
		"front",
		"back",
		"top",
		"bottom"
	};
	vec3 g_sideNormals[6] = {
		{1,0,0},	//1: right
		{-1,0,0},	//2: left
		{0,1,0},	//3: front
		{0,-1,0},	//4: back
		{0,0,1},	//5: top
		{0,0,-1}	//6: bottom
	};
};

#endif
