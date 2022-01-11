#ifndef GUI4FUNCTORS_GUARD
#define GUI4FUNCTORS_GUARD
#include "gui/Gui4.h"
#include "util/functor.h"

class GUI4mouseOverHandler:public functor{
	public:
	GUI4mouseOverHandler();
};
class GUI4mouseClickHandler:public functor{
	public:
	GUI4mouseClickHandler();
	vec2i clickPos;
};


#endif