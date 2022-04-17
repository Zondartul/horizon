#ifndef AICONTROLLER_GUARD
#define AICONTROLLER_GUARD

#include "game.h"
#include "node_graph.h"

enum AIState{   //high-level AI state
    AI_STATE_NONE,   //haven't decided on an action
    AI_STATE_IDLE,   //wait and be bored
    AI_STATE_WANDER, //walk around aimlessly
    AI_STATE_FOLLOW  //follow player
};

enum AISubstate{    //low-level AI state
    AI_SUBSTATE_THINK, //waiting for calculation to complete
    AI_SUBSTATE_TURN,  //smoothly turn to face a direction
    AI_SUBSTATE_WALK  //walk towards a destination
};

string toString(AIState state);
string toString(AISubstate substate);

struct SubstateWalkData{
    vec3 src;
    vec3 dest;
    nav_path path;
};

class AIcontroller:public eventListener{
	public:
	elastic_ptr<characterController> character;
	elastic_ptr<characterController> player;

	//state machine
	AIState state = AI_STATE_NONE;
	AISubstate substate = AI_SUBSTATE_THINK;
	int taskTime = 0;
	int sleepTime = 0;
	SubstateWalkData walkdata;

	vec3 debugTargetPos = vec3(0,0,0);
	string debugString = "";
	float walkSpeed = 0.05f;
	float turnSpeed = 45*d2r;

	AIcontroller(characterController *character);
	~AIcontroller();
	string toString2();
	void onEvent(eventKind event);
	void think();
};

#endif
