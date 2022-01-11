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

inputControllerKind::inputControllerKind(){
	mousecapture=forward=backward=left=right=up=down=false;
	//character = 0;
	speed = 0.07;
	flyspeed = 0.05;//0.25;
}
inputControllerKind::~inputControllerKind(){}
/*
void resetBox(){
	if(physbodies.size()){
		physbodies[0]->setPos(camera.pos+((vec3f){2,0,0}).rotate(camera.rot));
		physbodies[0]->setRot(camera.rot);
		physbodies[0]->setVel({0,0,0});
		physbodies[0]->setAngVel({0,0,0});
	}
}
*/

void inputControllerKind::think(){
/*
	if(character){
		vec3f dpos = speed*((vec3f){forward-backward,left-right,up-down}).rotate(camera.rot);
		character->setWalkDir(dpos);
		if(up){character->jump(0.5);}
		camera.setPos(character->getEyePos());
	}else{
		vec3f dpos = flyspeed*((vec3f){forward-backward,left-right,up-down}).rotate(camera.rot);
		setPos(camera.pos+dpos);
	}
	*/
	vec3f dpos = flyspeed*((vec3f){forward-backward,left-right,up-down}).rotate(camera.rot);
	setPos(camera.pos+dpos);
	//resetBox();
	//printf("(%f,%f,%f),<(%f,%f,%f)\n",camera.pos.x,camera.pos.y,camera.pos.z,camera.rot.x,camera.rot.y,camera.rot.z);
}
void inputControllerKind::setPos(vec3f pos){
	/* if(character){
		character->setPos(pos);
		camera.setPos(character->getEyePos());
	}else{
		camera.setPos(pos);
	} */
	camera.setPos(pos);
}
void inputControllerKind::aimRelative(vec3f aim){
	float aimspeed = 0.2;
	camera.setRot(camera.rot + aim*aimspeed);
}
void inputControllerKind::aim(vec3f aim){
	camera.setRot(aim);
}
void inputControllerKind::toggleMouseCapture(){
	mousecapture = !mousecapture;
	printf("mousecapture = %d\n",mousecapture);
	setMouseRelativeMode(mousecapture);
}

/*
void spawnBox(){
	vec3f pos = camera.pos+camera.forward()*camera.eyetrace(2.0)+(vec3f){0,0,0.75};
	new physbody(
	pos,//camera.pos+((vec3f){1,0,0}).rotate(camera.rot),
	(vec3f){0,0,0},
	(vec3f){1,1,1}*0.75,
	getModel("box"));
}
*/

/*
void experiment1(){
	vec3f O = camera.pos+camera.forward()*1;
	vec3f A = O+(vec3f){1,0,0};
	vec3f B = O+(vec3f){0,1,0};
	vec3f C = O+(vec3f){0,0,1};
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
	if(keyboardCaptured){return;}
	if (event.type == EVENT_KEY_DOWN){
		string K = event.keyboard.key;
		//printf("[%s]",K.c_str());
		if(K == "Escape"){exit(0);}
		if(K == "E"){toggleMouseCapture();return;}
		if(K == "R"){setPos({0,0,0});aim({0,0,0});return;}
		if(K == "W"){forward = true;return;}
		if(K == "S"){backward = true;return;}
		if(K == "A"){left = true;return;}
		if(K == "D"){right = true;return;}
		if(K == "Space"){up = true;return;}
		if(K == "Left Ctrl"){down = true;return;}
		if(K == "F7"){camera.screenshot();return;}
		//if(K == "B"){spawnBox();return;}
		//if(K == "V"){resetBox();return;}
		//if(K == "F1"){debugDrawEnabled = !debugDrawEnabled;return;}
		//if(K == "F"){if(character){character->toggleFly();}return;}
		//if(K == "C"){if(character){character = 0;}else{character = new physCharacter();setPos(camera.pos);}return;}
		if(K == "T"){
			setPos(camera.pos+camera.forward()*camera.eyetrace(100)+(vec3f){0,0,0.5});
			//if(character){character->toggleFly();character->toggleFly();}
			return;
		}
		if(K == "Left Shift"){speed = 0.14;flyspeed = 0.5;return;}
		//if(K == "P"){physics = !physics;return;}
		//if(K == "G"){toggleGravity();return;}
		//if(K == "Keypad 7"){experiment1();}
			
		printf("[%s]",K.c_str());
	
	}
		if (event.type == EVENT_KEY_UP){
		string K = event.keyboard.key;
		//printf("[/%s]",K.c_str());
		if(K == "W"){forward = false;return;}
		if(K == "S"){backward = false;return;}
		if(K == "A"){left = false;return;}
		if(K == "D"){right = false;return;}
		if(K == "Space"){up = false;return;}
		if(K == "Left Ctrl"){down = false;return;}
		if(K == "Left Shift"){speed = 0.07;flyspeed = 0.25;return;}
	}
	if (event.type == EVENT_MOUSE_MOVE){
		if(mousecapture){
			int xrel = event.mousemove.diff.x;
			int yrel = event.mousemove.diff.y;
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

inputControllerKind inputController;


void captureKeyboard(eventListener *L){keyboardCaptured = true;}
void releaseKeyboard(){keyboardCaptured = false;}
