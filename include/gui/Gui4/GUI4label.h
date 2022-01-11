#ifndef GUI4LABEL_GUARD
#define GUI4LABEL_GUARD
#include "gui/GUI4/GUI4base.h"

class GUI4label:public GUI4base{
	public:
	string text;
	vec2i text_origin;
	GUI4label &setText(string text);
	virtual void render();
	virtual void checkSizeToContents();
};

#endif