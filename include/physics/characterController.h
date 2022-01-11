#ifndef CHARACTERCONTROLLER_GUARD
#define CHARACTERCONTROLLER_GUARD

#include "vec.h"
#include "entity.h"
#include "event.h"
#include <memory>
using std::weak_ptr;

class characterController:public eventListener, public elastic_ptr_anchor{
	public:
	vec3 targetVelocity = vec3(0,0,0);
	vec3 targetDir = vec3(0,0,0);
	//float size = 0.5f;
	bool fly = false;  //when fly=false, only x,y of tV are effective
	bool jump = false; //same as fly when on ground.
	bool onGround = false;
	elastic_ptr<entity> E;
	//characterController();
	characterController(entity *newE=0);
	~characterController();
	void onEvent(eventKind event);
	void think();
};

#endif
