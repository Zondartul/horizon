#ifndef PHYSICS_GUARD
#define PHYSICS_GUARD
#include "vec.h"
#include "modelLoader.h"
struct physbody{
	//vec3f pos;
	//vec3f rot;
	model M;
	physbody(vec3f pos, vec3f rot, vec3f scale, model M);
	void setPos(vec3f pos);
	void setRot(vec3f rot);
	void setScale(vec3f scale);
	void setVel(vec3f vel);
	void setAngVel(vec3f angvel);
	vec3f getPos();
	vec3f getRot();
	vec3f getScale();
	vec3f getVel();
	vec3f getAngVel();
};

extern vector<physbody*> physbodies;
extern bool debugDrawEnabled;

bool raytrace(vec3f start, vec3f end, physbody **body_out, float *distance);
void renderPhysics();
void initPhysics();
void printPhysics();
void physicstick();

void spawnTerrain(string heightmapname, string texturename, vec3f scale, vec3f pos);
void toggleGravity();
#endif