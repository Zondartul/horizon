#ifndef GUI5BEHAVIORMOVABLE_GUARD
#define GUI5BEHAVIORMOVABLE_GUARD
#include "gui/Gui5/behaviors/GUI5behavior.h"

//This behavior allows the elements to be moved by 
//clicking and dragging their body.
class GUI5behaviorMovable:public GUI5behavior{
	friend class GUI5base;
	bool movable;			//element can be moved by dragging it
	bool moving;			//element is being moved right now
	vec2i moveOffset;		//used for moving
	public:
	GUI5behaviorMovable();
	GUI5behaviorMovable &setMovable(bool movable);
	void run(GUI5base *B, message *msg);
};
#endif