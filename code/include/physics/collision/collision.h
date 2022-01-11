#ifndef COLLISION_GUARD
#define COLLISION_GUARD
#include "vec.h"
#include <vector>
#include <utility>
#include "octree.h"
using std::pair;
using std::vector;
class entity;
#include "render.h" //for debug drawing
#include "rmodel.h"
#include "editmodel.h"
#include <functional>
using std::function;

struct collisionOptions{
	bool separate;
	bool resolve;
};

enum collisionbodytype {BODY_STATIC, BODY_DYNAMIC, BODY_TRIGGER, BODY_NOCOLLIDE};
struct collisioninfo;
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
	string bodyname;
	string name();
	vec3 pos = vec3(0,0,0);
	vec3 vel = vec3(0,0,0);
	vec3 gravity = vec3(0,0,0);
	float mass = 1;
	float restitution = 0;
	float friction = 0;
	//AABB aabb; //a bounding box is needed for octree stuff.
	octree_visitor *ov = 0;

	//CALLBACKS!
	function<void(collisioninfo*)> onCollision = 0;

	collisionbody();
	virtual ~collisionbody(); //is this how you make stuff polymorphic?

	rmodel *rm = 0;
	virtual void render(renderOptions *options);
	//accessors
	void setAABB(AABB aabb); //also updates the debug rmodel
	AABB getAABB();

	protected: //so second children can access it
	AABB aabb;  //needed for octree stuff.
};

class collisionbodyPoint:public collisionbody{
    public:
    collisionbodyPoint();
	collisionbodyPoint(vec3 p);
    virtual ~collisionbodyPoint(){}
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
	collisionbodyRay() = default;
	collisionbodyRay(vec3 from, vec3 dir);
};

class collisionbodyPlane:public collisionbody{
	public:
	collisionbodyPlane() = default;
	collisionbodyPlane(vec3 n, float offset);
	collisionbodyPlane(vec3 A, vec3 B, vec3 C);
	vec3 normal;
	float offset;
};

struct row_acessor{
	vector<float> *grid;
	int column;
	int row_size;
	float& operator[](int Iy);
};
#include <map>
using std::map;
struct tile;


struct gridKind;
enum tileShape {TILE_DIAG = 1, TILE_ALTERNATING_DIAG, TILE_BILINEAR};

struct tile{
	gridKind *grid = 0;
	tileShape shape = TILE_DIAG;
	int Ix_A, Iy_A;
	int Ix_B, Iy_B;
	int Ix_C, Iy_C;
	int Ix_D, Iy_D;

	vec3 vA();
	vec3 vB();
	vec3 vC();
	vec3 vD();

	float yA();
	float yB();
	float yC();
	float yD();

	float height(vec2 pos);	
	vec2 gradient(vec2 pos);
	vec3 normal(vec2 pos);
	vec3 getDiagPointOnLine(vec2 p1, vec2 p2);
};

struct gridKind{
	map<int,vec3> debugVerts;
	vector<float> height_grid;
	float x_step, y_step;
	int x_point_count, y_point_count;
	int x_tile_count, y_tile_count;
	row_acessor operator[](int Ix);
	vec3 point(int Ix, int Iy);
	
	gridKind() = default;
	gridKind(int numx, int numy, float xstep, float ystep);
	
	float height(vec2 pos);   //pos is relative to the start of the grid
	vec2 gradient(vec2 pos);  //pos is relative
	vec3 normal(vec2 pos);	  //pos is relative
	tile getTile(vec2 pos);   //pos is relative
	vec2 getEnd();		//returns the maximum relative pos that is still on this grid
};

class collisionbodyTerrain:public collisionbodyAABB{
	public:
	e_model *em = 0;
	void generateGridFromModel(int numx, int numy);
	void updateGridFromModel();
	gridKind grid;


	collisionbodyTerrain(AABB aabb):collisionbodyAABB(aabb){}
};

//stores info of the form "some point of body A is at distance D
//from another point of body B", not necessarily vertices.


//pos is the position of "the penetrating point" i.e. some arbitrary point within the intersection manifold.
//normal is the normal of the surface of body2 where penetration occured (points away from body2)

struct collisionpoint{
	vec3 pos;			//where the penetrating point is
	vec3 normal;		//the normal the penetrated surface of body2
	vec3 penetration;	//vector from point of penetration to penetrating point.
						//moving body2 by <vec3 penetration> should separate the bodies.
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

//a collisioninfo shall indicate how body1 penetrates body2
//where body1 is the actor and body2 is acted upon
struct collisioninfo{
	//collisionParty cp[2];
	collisionbody *body1 = 0;
	collisionbody *body2 = 0;
	public:
	collisionpoint c_to_c;	//center to center separation
	vector<collisionpoint> cpts; //additional collision points
	//collisionpoint AinB;
	//collisionpoint BinA;
};

collisioninfo *reverseCollision(collisioninfo *ci);

//cast a raytrace from point <from> in direction <dir> while optionally
//ignoring all entities in list <ignore>.
collisioninfo *raytrace(vec3 from, vec3 dir,const vector<entity *> &ignore={});

void printCollisionbody(collisionbody *body);
void printCollision(collisioninfo *col);

#include "broadphase.h"
#include "narrowphase.h"
#include "resolution.h"

#endif
