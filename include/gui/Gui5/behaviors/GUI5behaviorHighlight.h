#ifndef GUI5BEHAVIORHIGHLIGHT_GUARD
#define GUI5BEHAVIORHIGHLIGHT_GUARD
#include "gui/Gui5/behaviors/GUI5behavior.h"

//makes the element use different colors when it has a mouseover.
//when mouseover begins, any colors specified in the newColors map
//will replace the element's original colors.
//when mouseover ends, original colors will be put back in place.
class GUI5behaviorHighlight:public GUI5behavior{
	friend class GUI5base;
	map<string, color4i> newColors;
	bool mouseover;
	public:
	GUI5behaviorHighlight();
	void run(GUI5base *B, message *msg);
};


#endif