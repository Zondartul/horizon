#ifndef GUI5SCROLLBAR_GUARD
#define GUI5SCROLLBAR_GUARD
#include "gui/Gui5/GUI5base.h"

class GUI5scrollbar:public GUI5base{
	protected:
	bool vertical;
	bool horizontal;
	vec2i max;
	vec2i min;
	vec2i cur;
	public:
	GUI5scrollbar();
	GUI5scrollbar &setVertical(bool v);
	GUI5scrollbar &setHorizontal(bool h);
	GUI5scrollbar &setVerticalLimits(int start, int end);
	GUI5scrollbar &setHorizontalLimits(int start, int end);
	GUI5scrollbar &setVerticalPos(int pos);
	GUI5scrollbar &setHorizontalPos(int pos);
	void render();
	void layout();
	void receiveMessageExtra(message *msg);
};

#endif