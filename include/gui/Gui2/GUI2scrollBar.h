#ifndef GUI2SCROLLBAR_GUARD
#define GUI2SCROLLBAR_GUARD
#include "gui/Gui2/GUI2base.h"
#include "gui/Gui2/GUI2scrollslidey.h"
#include "gui/Gui2/GUI2button.h"
class GUI2scrollBar:public GUI2base
{
	public:
	bool vertical;
	GUI2button *btnup;
	GUI2button *btndn;
	GUI2button *track;
	GUI2scrollslidey *slidey;
	vec2i insideSize;
	vec2i sizeOff;
	float Amin; //slider position
	float Amax;
	int Aoffset;
	static void btnupFunc(void *arg);
	static void btndnFunc(void *arg);
	static void updateBar(void *arg);
	static int propagateScroll(GUI2base* obj, void* arg, int rec);
	void setSize(int x, int y);
	GUI2scrollBar();
	void setParent(GUI2base* obj);
	void invalidate(vec2i newPos, vec2i newSize);
	void render(void *arg);
};
#endif