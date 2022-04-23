#ifndef ENTITY_GUARD
#define ENTITY_GUARD
#include "vec.h"
#include "event.h"
#include "collision.h"
#include "render.h"
#include <map>
#include <list>
#include <vector>
#include <string>
#include "elastic_ptr.h"
using std::map;
using std::list;
using std::vector;
using std::string;
struct octree_visitor;

class entity;

extern list<entity*> g_entities;

class entity:public elastic_ptr_anchor{
	public:
	string name;
	string group; //for multi-entity commands
	collisionbody *body = 0;
	renderable *r = 0;
	entity();
	~entity();
	vec3 getPosition();		void setPosition(vec3 pos);
	vec3 getVelocity();		void setVelocity(vec3 vel);
	float getMass();		void setMass(float mass);
	float getBouncyness();	void setBouncyness(float B);
	float getFriction();	void setFriction(float friction);
	vec3 getGravity();		void setGravity(vec3 gravity);
	vec3 getColor();		void setColor(vec3 color);
};
void addEntity(entity *E);
void removeEntity(entity *E);
entity *lastEntity(); //returns the last created entity

#define decl_component(T,name)						\
	class component_##name:public component{		\
		public:										\
		T val;										\
		component_##name() = delete;				\
		component_##name(T val, entity *ent);		\
		virtual void setup();						\
	}
	
#define imp_component0(T,name)								\
	component_##name::component_##name(T val, entity *ent){	\
		this->val = val;									\
		this->ent = ent;									\
		setup();											\
	}														\
	void component_##name::setup(){}
	
#define imp_component1(T,name)								\
	component_##name::component_##name(T val, entity *ent){	\
		this->val = val;									\
		this->ent = ent;									\
		setup();											\
	}

	
#define hasComponent(x,y) (x)->components.count(ecs_##y)
#define getComponent(x,y) ((component_##y*)((x)->components[ecs_##y]))
#define addComponent(E,x,y) {auto val = (y); (E)->components[ecs_##x] = new component_##x(val,(E));}

#endif