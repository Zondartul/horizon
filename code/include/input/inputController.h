#ifndef INPUTCONTROLLER_GUARD
#define INPUTCONTROLLER_GUARD
#include "vec.h"
#include "event.h"
#include "characterController.h"

class inputControllerKind:public eventListener{ //if there is a user (not headless), this does his thing.
	public:
	bool forward,backward,left,right,up,down;
	vec3 velocity;
	float targetspeed,warp;
	bool accelerating;
	struct{
		float fly_fast;
		float fly_normal;
		float fly_slow;
		float fly_acceleration;
		float walk_fast;
		float walk_normal;
		float walk_slow;
		float jump;
	}speeds;

	bool mousecapture;
	inputControllerKind();
	~inputControllerKind();
	void think();
	void setPos(vec3 pos);
	void aimRelative(vec3 aim);
	void aim(vec3 aim);
	void toggleMouseCapture();
	void enableMouseCapture();
	void disableMouseCapture();
	void onEvent(eventKind event);
	elastic_ptr<characterController> character;
};

void captureKeyboard(eventListener *L);
void releaseKeyboard();

//extern inputControllerKind *g_inputController;

struct gs_inputControllerKind {
	bool g_keyboardCaptured;
	inputControllerKind* g_inputController;
};

#endif
