#ifndef ENTITY_GUARD
#define ENTITY_GUARD
#include "vec.h"
#include <map>
#include <list>
#include <vector>
using std::map;
using std::list;
using std::vector;
struct rmodel;
struct texture;

class component;
enum componentType{
	ecs_position,
	ecs_rmodel,
	ecs_texture
};

class entity{
	public:
	map<componentType, component*> components;
	entity();
};

extern list<entity*> entities;

class component{
	public:
	entity *ent;
};

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


decl_component(vec3, position);
decl_component(rmodel*, rmodel);
decl_component(texture*, texture);
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
class ecs_render_system_kind{
	public:
	void render();
};
extern ecs_render_system_kind ecs_render_system;
#endif