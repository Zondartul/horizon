#ifndef ECS_GUARD
#define ECS_GUARD
#include "event.h"

struct renderLayer;
class ecs_render_system_kind:public eventListener{
	public:
	renderLayer *ecs_render_layer;
	ecs_render_system_kind();
	void onEvent(eventKind event);
};

extern bool physicsOn;
class ecs_physics_system_kind:public eventListener{
	public:
	ecs_physics_system_kind();
	void onEvent(eventKind event);
};

class ecs_collision_system_kind:public eventListener{
	public:
	ecs_collision_system_kind();
	void onEvent(eventKind event);
};

struct entity;
void applyGravity(entity *E);
void applyVelocity(entity *E);
bool isSane(entity *E);

#endif