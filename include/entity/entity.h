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
//struct rmodel;
//struct texture;
struct octree_visitor;

//class component;
/*
enum componentType{
	//ecs_position,
	//ecs_rmodel,
	//ecs_texture,
	//ecs_velocity,
	//ecs_physics,
	//ecs_gravity,
	ecs_collider,
	//ecs_mass,
	//ecs_physproperties,
	//ecs_color,
	//ecs_octree_visitor,
	ecs_renderable,
};
*/
struct entity;

extern list<entity*> entities;

class entity:public elastic_ptr_anchor{
	public:
	string name;
	string group; //for multi-entity commands
	//map<componentType, component*> components;
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
//class component{
//	public:
//	entity *ent;
//};

//struct physProperties{
//	float mass;
//	float bouncyness;
//	float friction;
//	physProperties() = default;
//	physProperties(float mass, float bouncyness, float friction);
//};

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


//decl_component(vec3, position);
//decl_component(rmodel*, rmodel);
//decl_component(texture*, texture);
//decl_component(vec3, velocity);
//decl_component(bool, physics);
//decl_component(vec3, gravity);
//decl_component(collisionbody*, collider);
//decl_component(float, mass);
//decl_component(physProperties, physproperties);
//decl_component(vec3, color);
//decl_component(octree_visitor*, octree_visitor);
//decl_component(renderable*, renderable);
/*
class component_position{
	public:
	vec3 pos;
	component_position(vec3 pos, entity *ent);
};
class component_rmodel{
	public:
	rmodel *rm;
	component_position(rmodel *rm, entity *ent);
};
class component_texture{
	public:
	texture *t;
	component_position(texture *t, entity *ent);
};
*/



//collisioninfo *raytrace(vec3 from, vec3 dir);
//extern ecs_physics_system_kind ecs_physics_system;

#endif