#ifndef GUI5BEHAVIORCHILDREN_GUARD
#define GUI5BEHAVIORCHILDREN_GUARD
#include "gui/Gui5/behaviors/GUI5behavior.h"

//passes the message to element's children. No other function.
class GUI5behaviorChildren:public GUI5behavior{
	friend class GUI5base;
	public:
	void run(GUI5base *B, message *msg);
};

#endif