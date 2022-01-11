#ifndef GUI5LABEL_GUARD
#define GUI5LABEL_GUARD
#include "gui/Gui5/GUI5base.h"

class GUI5label:public GUI5base{
	public:
	string text;
	GUI5label &setText(string text);
	void render();
	void layout();
};

#endif