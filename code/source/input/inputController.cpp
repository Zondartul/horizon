#include "inputController.h"
#include "camera.h"
//#include "stdio.h"
#include <string>
using std::string;
//#include "globals.h"
//#include "selfaware.h"
#include "mouse.h"
#include "globals.h"
//#include "physics.h"
//#include "physCharacter.h"
//#include "paint.h"
//extern bool physics;
#include "stdlib.h"
#include "simplemath.h"
#include "stringUtils.h"
#include "entity.h"
#include "input.h"
/*
//mousecapture:

  mouse keyboard
	|      |
	v      v
	controller
	|		|
	v		v

//no mousecapture:

  mouse keyboard
    |		|
	v		v
	---GUI---
	|		|
	v		v
	controller

*/

inputControllerKind::inputControllerKind(){
	mousecapture=forward=backward=left=right=up=down=false;
	//character = 0;
	speeds.fly_fast			= 0.25;
	speeds.fly_normal		= 0.05;
	speeds.fly_slow			= 0.01;
	speeds.fly_acceleration	= 0.01;
	speeds.walk_fast		= 0.01;
	speeds.walk_normal		= 0.003;
	speeds.walk_slow		= 0.001;
    speeds.jump             = 0.1;

	velocity = {0,0,0};
	targetspeed = speeds.fly_normal;
	accelerating = false;
	warp = 1;
	inputChannel->addListener(this);
	globalChannel->addListener(this);
	character = 0;
}
inputControllerKind::~inputControllerKind(){}
/*
void resetBox(){
	if(physbodies.size()){
		physbodies[0]->setPos(camera.pos+((vec3){2,0,0}).rotate(camera.rot));
		physbodies[0]->setRot(camera.rot);
		physbodies[0]->setVel({0,0,0});
		physbodies[0]->setAngVel({0,0,0});
	}
}
*/
#include "glm/gtx/projection.hpp"
void inputControllerKind::think(){
/*
	if(character){
		vec3 dpos = speed*((vec3){forward-backward,left-right,up-down}).rotate(camera.rot);
		character->setWalkDir(dpos);
		if(up){character->jump(0.5);}
		camera.setPos(character->getEyePos());
	}else{
		vec3 dpos = flyspeed*((vec3){forward-backward,left-right,up-down}).rotate(camera.rot);
		setPos(camera.pos+dpos);
	}
	*/
	//vec3 targetvel = warp*targetspeed*vec3(forward-backward,left-right,up-down);

	if(character && character->E){
		float horSpeed = targetspeed;


		vec3 flyDir = rotate(vec3(forward-backward,left-right,0),d2r*camera.rot);
		vec3 horDir = normalizeSafe(vec3(flyDir.x,flyDir.y,0));//normalizeSafe(flyDir-proj(flyDir,vec3(0,0,1)));
		velocity = horSpeed*horDir+speeds.jump*vec3(0,0,up-down);
	}else{
		vec3 targetvel = warp*targetspeed*(rotate(vec3(forward-backward,left-right,up-down),d2r*camera.rot));
		if(length(targetvel) && accelerating){warp = 10.f;}//{warp = warp *(1+(0.25f)/60.f);}
		else{warp = 1.f;}
		vec3 dv = targetvel - velocity;
		//weird-ass acceleration formula
		//tl;dr:
		//if dv is less than max acceleration, just use dv.
		//if dv is more than max acceleration,
		//		then limit dv to max acceleration, scaled so things that are already fast accelerate quicker.
		/*if(length(dv) > speeds.fly_acceleration){
			//dv = normalize(dv)*speeds.fly_acceleration*(max(velocity.length()/speeds.fly_normal,1));
			vec3 dir = normalize(dv);
			float acc = speeds.fly_acceleration;
			float vel_len = length(velocity);
			float fly_ratio = vel_len / speeds.fly_normal;
			float acc_coeff = max(fly_ratio,1);
			dv = dir*acc*acc_coeff;
		}*/
		velocity = velocity+dv;
		//velocity = targetvel;
		vec3 dpos = velocity;
	}
	//apply velocity and camera position
	if(character && character->E){
		character->targetVelocity = velocity;
		character->jump = up;
		auto E = character->E;

		vec3 cp = camera.pos;
		vec3 cf = camera.forward();
		vec3 cu = camera.up();
		character->targetDir = toVec3Angle(setZ(camera.forward(),0));
		vec3 bp = E->body->pos;
		//first-person camera
		//camera.setPos(bp);
		//third-person camera
		float sh = 0.9f;//shoulder height
		float sl = 1.f;//shoulder length
		//vec3 shoulder = bp+normalizeSafe(cross(cf,vec3(1,0,0)))*sh-cf*sl;
		vec3 bf = normalize(setZ(cf,0));//body forward
		vec3 camOffset = vec3(0,0,sh);
		vec3 camArm = vec3(0,-sl,0.2);
		vec3 up = vec3(0,0,1);
		vec3 shoulder = bp+camOffset.y*bf+camOffset.z*up+camArm.y*cf+camArm.z*cu;
		
		bool thirdperson = false;
		if(thirdperson){
			camera.setPos(shoulder);
		}else{
			float hh = 0.7f; //head height
			camera.setPos(bp+vec3(0,0,hh));
		}
	}else{
		setPos(camera.pos+velocity);
	}
	//resetBox();
	//printf("(%f,%f,%f),<(%f,%f,%f)\n",camera.pos.x,camera.pos.y,camera.pos.z,camera.rot.x,camera.rot.y,camera.rot.z);
}
void inputControllerKind::setPos(vec3 pos){
	/* if(character){
		character->setPos(pos);
		camera.setPos(character->getEyePos());
	}else{
		camera.setPos(pos);
	} */
	camera.setPos(pos);
}
void inputControllerKind::aimRelative(vec3 aim){
	float aimspeed = 0.2;
	camera.setRot(camera.rot + aim*aimspeed);
}
void inputControllerKind::aim(vec3 aim){
	camera.setRot(aim);
}
void inputControllerKind::toggleMouseCapture(){
	//mousecapture = !mousecapture;
	//if(mousecapture){inputChannel->moveListenerToFront(this);} //temporary hack
	//else{inputChannel->moveListenerToBack(this);}
	//printf("mousecapture = %d\n",mousecapture);
	//setMouseRelativeMode(mousecapture);
	mousecapture? disableMouseCapture() : enableMouseCapture();
}
void inputControllerKind::enableMouseCapture(){
	printf("mouse captured\n");
	mousecapture = true;
	inputChannel->moveListenerToFront(this);
	setMouseRelativeMode(true);
}
void inputControllerKind::disableMouseCapture(){
	printf("mouse released\n");
	mousecapture = false;
	inputChannel->moveListenerToBack(this);
	setMouseRelativeMode(false);
}
/*
void spawnBox(){
	vec3 pos = camera.pos+camera.forward()*camera.eyetrace(2.0)+(vec3){0,0,0.75};
	new physbody(
	pos,//camera.pos+((vec3){1,0,0}).rotate(camera.rot),
	(vec3){0,0,0},
	(vec3){1,1,1}*0.75,
	getModel("box"));
}
*/

/*
void experiment1(){
	vec3 O = camera.pos+camera.forward()*1;
	vec3 A = O+(vec3){1,0,0};
	vec3 B = O+(vec3){0,1,0};
	vec3 C = O+(vec3){0,0,1};
	renderContext{
		setColor({255,0,0});
		drawLine3D(O,A);
		setColor({0,255,0});
		drawLine3D(O,B);
		setColor({0,0,255});
		drawLine3D(O,C);
	}
}
*/

bool keyboardCaptured;
void inputControllerKind::onEvent(eventKind event){
	if (event.type == EVENT_FRAME){
		think();
	}
	if(keyboardCaptured){return;}
	if (event.type == EVENT_KEY_DOWN){
		string K = event.keyboard.key;
		//printf("[%s]",K.c_str());
		if(K == "Escape"){event.maskEvent();exit(0);}
		if(K == "E"){event.maskEvent();toggleMouseCapture();return;}
		//if(K == "R"){event.maskEvent();setPos({0,0,0});aim({0,0,0});entities.clear();return;}
		if(K == "W"){event.maskEvent();forward = true;return;}
		if(K == "S"){event.maskEvent();backward = true;return;}
		if(K == "A"){event.maskEvent();left = true;return;}
		if(K == "D"){event.maskEvent();right = true;return;}
		if(K == "Space"){event.maskEvent();up = true;return;}
		if(K == "Left Ctrl"){event.maskEvent();down = true;return;}
		if(K == "F7"){event.maskEvent();camera.screenshot();return;}
		//if(K == "B"){spawnBox();return;}
		//if(K == "V"){resetBox();return;}
		//if(K == "F1"){debugDrawEnabled = !debugDrawEnabled;return;}
		//if(K == "F"){if(character){character->toggleFly();}return;}
		//if(K == "C"){if(character){character = 0;}else{character = new physCharacter();setPos(camera.pos);}return;}
		if(K == "C"){if(character){event.maskEvent(); character = 0;}}
		if(K == "T"){
			event.maskEvent();
			if(!character){
				float dist = 100;
				auto *ci = camera.eyetrace(false);
				if(ci){
					float dist2 = ci->c_to_c.depth;
					if(dist2 < dist){dist = dist2;}
				}
				dist -= 0.3f;
				setPos(camera.pos+camera.forward()*dist/*+(vec3){0,0,0.5}*/);
			}
			//if(character){character->toggleFly();character->toggleFly();}
			return;
		}
		if(K == "Left Shift"){event.maskEvent();targetspeed = speeds.fly_fast;accelerating = true; speeds.jump = 0.2f; return;}
		//if(K == "P"){physics = !physics;return;}
		//if(K == "G"){toggleGravity();return;}
		//if(K == "Keypad 7"){experiment1();}

		printf("[%s]",K.c_str());
	}
	if (event.type == EVENT_KEY_UP){
		string K = event.keyboard.key;
		//printf("[/%s]",K.c_str());
		if(K == "W"){event.maskEvent();forward = false;return;}
		if(K == "S"){event.maskEvent();backward = false;return;}
		if(K == "A"){event.maskEvent();left = false;return;}
		if(K == "D"){event.maskEvent();right = false;return;}
		if(K == "Space"){event.maskEvent();up = false;return;}
		if(K == "Left Ctrl"){event.maskEvent();down = false;return;}
		if(K == "Left Shift"){event.maskEvent();targetspeed = speeds.fly_normal;accelerating = false; speeds.jump = 0.1f; return;}
	}
	if (event.type == EVENT_MOUSE_MOVE){
		if(mousecapture){
			event.maskEvent();
			int xrel = event.mousemove.diff.x;
			int yrel = event.mousemove.diff.y;
			//printf("xrel = %d, yrel = %d\n",xrel,yrel);
			aimRelative({0,-yrel,xrel});
		}
		return;
	}
}
/*
void inputControllerKind::sdl_event(SDL_Event event){
	if (event.type == SDL_KEYDOWN){
		string K = SDL_GetKeyName(event.key.keysym.sym);
		printf("[%s]",K.c_str());
		if(K == "Escape"){exit(0);}
		if(K == "E"){toggleMouseCapture();}
		if(K == "R"){setPos({0,0,0});aim({0,0,0});}
		if(K == "W"){forward = true;}
		if(K == "S"){backward = true;}
		if(K == "A"){left = true;}
		if(K == "D"){right = true;}
		if(K == "Space"){up = true;}
		if(K == "Left Ctrl"){down = true;}
		if(K == "F7"){camera.screenshot();}
	}
	if (event.type == SDL_KEYUP){
		string K = SDL_GetKeyName(event.key.keysym.sym);
		if(K == "W"){forward = false;}
		if(K == "S"){backward = false;}
		if(K == "A"){left = false;}
		if(K == "D"){right = false;}
		if(K == "Space"){up = false;}
		if(K == "Left Ctrl"){down = false;}
	}
	if (event.type == SDL_MOUSEMOTION){
		if(mousecapture){
			int xrel = event.motion.xrel;
			int yrel = event.motion.yrel;
			aimRelative({0,-yrel,xrel});
		}
	}
} */

inputControllerKind *inputController;


void captureKeyboard(eventListener *L){keyboardCaptured = true;}
void releaseKeyboard(){keyboardCaptured = false;}
