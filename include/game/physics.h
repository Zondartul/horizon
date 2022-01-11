#ifndef PHYSICS_GUARD
#define PHYSICS_GUARD
#include "../resource/models.h"
#include "../util/globals.h"
#include "../math/quaternions.h"
extern int physMaxId;
#define PHYSBODY_MAX_CHILDREN 11
class physBody
{
	public:
	model *mdl;
	vec pos;
	vec vel;
	vec acc;
	quat orient;
	quat angVel;
	double mass;
	double scale;
	vec3i color;
	unsigned char alpha;
	vec BScenter;//bounding sphere
	double BSradius;
	vec AABBmin;
	vec AABBmax;
	vec restAABBsize;
	vec restAABBoffset;
	char tracegroup; // 0 = no traces, 1 = phys traces
	ptrSniper<physBody> ptrs;
	int id;
	string name;
	int collisionCount;
	void (*onThink)(void*);
	physBody *parent;
	physBody *children[PHYSBODY_MAX_CHILDREN];
	void zero();
	physBody();
	physBody(model *M);
	void setPos(vec newpos);
	void setOrient(quat neworient);
	void setScale(double newscale);
	void setParent(physBody* prnt);
	void generateBoundingSphere();
	void updateAABB();
	void applyForce(vec force);
	~physBody();
	
};
extern physBody* EntLookAt;
extern vector<physBody*> AllPhysBodies;
class trace
{
	public:
	vec start;
	vec dir;
	bool hit;
	double dist;
	vec hitpos;
	physBody *hitObj;
	char tracegroup;
	trace();
	void scan();
};
void PhysicsTick();
vec GJKsupport(physBody *A, physBody *B, vec d);
model *GJKcollision(physBody *A, physBody *B);
#define EPA_TOLERANCE 0.001
vec EPAcollision(physBody *A, physBody *B, model *simplex);
vec CPcollision(physBody *A, physBody *B, vec plane);
int CPbestEdge(physBody *A, vec plane);
#endif








