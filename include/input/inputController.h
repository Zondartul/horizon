#ifndef INPUTCONTROLLER_GUARD
#define INPUTCONTROLLER_GUARD
#include "vec.h"
//#include "globals.h"
//#include "SDL2/SDL.h" //for event
#include "event.h"
//#include "physCharacter.h"

class inputControllerKind:public eventListener{ //if there is a user (not headless), this does his thing.
	public:
	bool forward,backward,left,right,up,down;
	float speed;
	float flyspeed;
	bool mousecapture;
	inputControllerKind();
	~inputControllerKind();
	void think();
	void setPos(vec3f pos);
	void aimRelative(vec3f aim);
	void aim(vec3f aim);
	void toggleMouseCapture();
	void onEvent(eventKind event);
	//physCharacter *character;
};

void captureKeyboard(eventListener *L);
void releaseKeyboard();

extern inputControllerKind inputController;

#endif