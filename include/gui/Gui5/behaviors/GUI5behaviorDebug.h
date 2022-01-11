#ifndef GUI5BEHAVIORDEBUG_GUARD
#define GUI5BEHAVIORDEBUG_GUARD
#include "gui/Gui5/behaviors/GUI5behavior.h"

class GUI5behaviorDebug:public GUI5behavior{
	friend class GUI5base;
	void run(GUI5base *B, message *msg);
};

#endif