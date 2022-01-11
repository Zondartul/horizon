#ifndef COLLISION_GUARD
#define COLLISION_GUARD
#include "vec.h"
#include <vector>
#include <utility>
#include "octree.h"
using std::pair;
using std::vector;
struct entity;

//struct physinfo{
//	vec3 vel;
//	float mass;
//	float restitution;
//	float friction;
//};
enum collisionbodytype {BODY_STATIC, BODY_DYNAMIC, BODY_TRIGGER, BODY_NOCOLLIDE};
//all entities that exist somewhere in space / have a position,
//also have a collision body.
class collisionbody{
	public:
	// collisions:
	// BODY_STATIC: generates collisions, moves dynamic bodies, but is not
	// 		moved itself.
	// BODY_DYNAMIC: generates collisions, moves dynamic bodies and is moved.
	// BODY_TRIGGER: generates collisions, but does not move dynamic bodies
	// 		and is not moved itself.
	// BODY_NOCOLLIDE: does not generate collisions.
	collisionbodytype type = BODY_STATIC;
	entity *E = 0;
	vec3 pos = vec3(0,0,0);
	vec3 vel = vec3(0,0,0);
	vec3 gravity = vec3(0,0,0);
	float mass = 1;
	float restitution = 0; 
	float friction = 0;
	AABB aabb; //a bounding box is needed for octree stuff.
	octree_visitor *ov = 0;
	//physinfo phys;
	collisionbody();
	virtual ~collisionbody(); //is this how you make stuff polymorphic?
};

class collisionbodyAABB:public collisionbody{
	public:
	//AABB aabb;
	collisionbodyAABB(AABB aabb);
	virtual ~collisionbodyAABB(){}	
};

class collisionbodyRay:public collisionbody{
	public:
	vec3 from;
	vec3 dir;
	collisionbodyRay(vec3 from, vec3 dir);
};

//stores info of the form "some point of body A is at distance D 
//from another point of body B", not necessarily vertices.
struct collisionpoint{
	vec3 pos;			//where the penetrating point is
	vec3 normal;		//the normal the penetrated surface
	vec3 penetration;	//vector from point of penetration to penetrating point
	float depth;		//length of penetration
};


/*
struct collisionParty{
	collisionbody *body;
	entity* ent;				//new phone who dis
	vec3 vel;
	vec3 pos;
	float mass;
	float friction;
	float restitution;
};
*/

struct collisioninfo{
	//collisionParty cp[2];
	collisionbody *body1;
	collisionbody *body2;
	public:
	collisionpoint c_to_c;	//center to center separation
	vector<collisionpoint> cpts; //additional collision points
	//collisionpoint AinB;
	//collisionpoint BinA;
};

bool canCollide(collisionbody *body1, collisionbody *body2);
collisioninfo *checkCollisionAABB_AABB(collisionbodyAABB *body1, collisionbodyAABB *body2);
collisioninfo *checkCollisionRay_AABB(collisionbodyRay *bodyRay, collisionbodyAABB *bodyAABB);


void resolve1Dcollision(float vel1, float mass1, float vel2, float mass2, float *vel1_result, float *vel2_result,float restitution);
float frictionFormula(float f1, float f2);
float restitutionFormula(float r1, float r2);

void resolveCollision(collisioninfo *col);

typedef pair<collisionbody*,collisionbody*> collisionpair;

struct broadphaseinfo{
	vector<collisionbody*> bodies;
	vector<collisionpair> pairs;
	void add(broadphaseinfo *bp2[8]);	   //add the entities and collisions from another bp
	void addJoin(broadphaseinfo *bp2[8]); //add and make pairs between the two sets
};

broadphaseinfo *getBroadphaseNodeOnly(octree_node *node);
broadphaseinfo *checkCollisionBroadphase(octree_node *node);
void broadphaseRender(broadphaseinfo *bp);

collisioninfo *reverseCollision(collisioninfo *ci);
collisioninfo *collisionCheckDispatch(collisionbody *body1, collisionbody *body2);
bool canCollide(collisionbody *body1, collisionbody *body2);
void pairwiseCollisionCheck(collisionbody *body1, collisionbody *body2);
collisioninfo *raytrace(vec3 from, vec3 dir);



#endif