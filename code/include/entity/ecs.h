#ifndef ECS_GUARD
#define ECS_GUARD
#include "event.h"
#include "render.h"
#include "collision.h"
extern bool physicsOn;
extern bool collisionOn;
extern bool texturingOn;

class ecs_render_system_kind;
class ecs_physics_system_kind;
class ecs_collision_system_kind;

struct ecs_kind{
	ecs_render_system_kind *render;
	ecs_physics_system_kind *physics;
	ecs_collision_system_kind *collision;
};
extern ecs_kind ecs;

class renderLayer;
extern renderLayer *ecs_render_layer;
extern renderLayer *ecs_render_layer_2d;

class ecs_render_system_kind:public eventListener{
	public:
	//renderLayer *ecs_render_layer;
	ecs_render_system_kind();
	void onEvent(eventKind event);
	//options
	renderOptions options;

	void applyRenderOptions();
};

struct physicsOptions{
	bool gravity;
};

class ecs_physics_system_kind:public eventListener{
	public:
	ecs_physics_system_kind();
	physicsOptions options;
	void onEvent(eventKind event);
};


class ecs_collision_system_kind:public eventListener{
	public:
	ecs_collision_system_kind();
	collisionOptions options;
	void onEvent(eventKind event);
};

class entity;
void applyGravity(entity *E);
void applyVelocity(entity *E);
bool isSane(entity *E);

#endif
