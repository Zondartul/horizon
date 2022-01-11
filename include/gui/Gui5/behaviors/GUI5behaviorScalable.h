#ifndef GUI5BEHAVIORSCALABLE_GUARD
#define GUI5BEHAVIORSCALABLE_GUARD
#include "gui/GUI5/behaviors/GUI5behavior.h"

//This behavior allows the elements to be resized by
//grabbing the edges and dragging
class GUI5behaviorScalable:public GUI5behavior{
	friend class GUI5base;
	bool resizable;			//element can be resized by grabbing the edges
	int resizeState;		//element is being resized right now
	int borderSize;			//size of the border
	bool cursorSet;
	public:
	GUI5behaviorScalable();
	GUI5behaviorScalable &setResizable(bool resizable);
	GUI5behaviorScalable &setBorderSize(int size);
	void run(GUI5base *B, message *msg);
};
#endif