#include "entity.h"
#include "collision.h"
#include "gadgets/frameprinter.h"
#include "physics/octree.h"

list<entity*> entities;

entity::entity(){
}

entity::~entity(){
	delete body;
	delete r;
}

vec3 entity::getPosition(){return body ? body->pos : vec3(0,0,0);}
void entity::setPosition(vec3 pos){
	body->pos = pos; 
	body->ov->moveTo(pos);
	if(r){r->pos = pos;}
}

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

void addEntity(entity *E){
	entities.push_back(E);
}
void removeEntity(entity *E){
	if(!E){return;}
	if(E->body){
		delete E->body;
	}
	
	for(auto I = entities.begin(); I != entities.end();){
		if(*I==E){I = entities.erase(I);}else{I++;}
	}
}

entity *lastEntity(){
	return entities.back();
}
//future ecs stuff:
//overal "dispatch" system that handles lists of similar components
//and calls other "systems" for each component.














