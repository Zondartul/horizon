#ifndef PHYSCHARACTER_GUARD
#define PHYSCHARACTER_GUARD
#include "vec.h"

class physCharacter{
	public:
	vec3f pos;
	bool fly;
	physCharacter();
	void setWalkDir(vec3f vel);
	void jump(float height);
	void toggleFly();
	void setVel(vec3f vel);
	void setPos(vec3f pos);
	void setRot(vec3f rot);
	void setSize(vec3f size);
	bool onGround();
	vec3f getEyePos();
	vec3f getPos();
};

//movetype:
// walking:
//  walk velocity
//  horizontal movement
//  full gravity
//
// falling:
//  no velocity or air control velocity
//  horizontal movement
//  full gravity
//
// swimming:
//  swim velocity
//  free movement
//  reduced gravity
//
// flying:
//  fly velocity
//  free movement
//  no gravity

//on collision w/nonliving:
//  phase
//  push_phase
//  push_stop (default)
//  stop

//on collision w/living
//  as_nonliving
//  push_hurt_phase
//  push_hurt_stop
//  push_crush

#endif
