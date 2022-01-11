#include "entity.h"
#include "collision.h"
#include "gadgets/frameprinter.h"
#include "physics/octree.h"
bool physicsOn = true;

list<entity*> entities;
//ecs_render_system_kind ecs_render_system;

//imp_component0(collisionbody*, collider); //noms the following:
//imp_component0(vec3, position);
//imp_component0(vec3, velocity);
//imp_component0(bool, physics);
//imp_component0(vec3, gravity);
//imp_component0(float, mass);
//imp_component0(physProperties,physproperties);
//imp_component0(octree_visitor*, octree_visitor);
//imp_component1(renderable*,renderable); //noms the following:
//imp_component1(rmodel*, rmodel);
//imp_component1(texture*, texture);
//imp_component0(vec3, color);

entity::entity(){
	entities.push_back(this);
}

entity::~entity(){
	delete body;
	delete r;
}

vec3 entity::getPosition(){return body ? body->pos : vec3(0,0,0);}
void entity::setPosition(vec3 pos){body->pos = pos; body->ov->moveTo(pos);}

vec3 entity::getVelocity(){return body ? body->vel : vec3(0,0,0);}
void entity::setVelocity(vec3 vel){body->vel = vel;}

float entity::getMass(){return body ? body->mass : 0;}
void entity::setMass(float mass){body->mass = mass;}

float entity::getBouncyness(){return body ? body->restitution : 1.f;}
void entity::setBouncyness(float B){body->restitution = B;}

float entity::getFriction(){return body ? body->friction : 1.f;}
void entity::setFriction(float friction){body->friction = friction;}

vec3 entity::getGravity(){return body ? body->gravity : vec3(0,0,0);}
void entity::setGravity(vec3 gravity){body->gravity = gravity;}

vec3 entity::getColor(){return r ? r->color: vec3(255,255,255);}
void entity::setColor(vec3 color){r->color = color;}

void removeEntity(entity *E){
	delete E;
	for(auto I = entities.begin(); I != entities.end();){
		if(*I=E){I = entities.erase(I);}else{I++;}
	}
}
//future ecs stuff:
//overal "dispatch" system that handles lists of similar components
//and calls other "systems" for each component.














