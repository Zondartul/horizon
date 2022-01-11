#ifndef GUI5WINDOW_GUARD
#define GUI5WINDOW_GUARD
#include "gui/Gui5/GUI5base.h"

class GUI5window:public GUI5base{
	public:
	string title;
	GUI5window();
	GUI5window &setTitle(string t);
	void layout();
	void render();
};

#endif