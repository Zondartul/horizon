#ifndef GUI5BEHAVIOR_GUARD
#define GUI5BEHAVIOR_GUARD
#include "util/messenger.h"

class GUI5base;

//behaviors are components that can be added to an element
//to allow certain kinds of event-based interactivity.
class GUI5behavior{
	friend class GUI5base;
	public:
	GUI5behavior();
	virtual void run(GUI5base *B, message *msg);
};


#endif