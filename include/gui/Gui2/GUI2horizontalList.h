#ifndef GUI2HORIZONTALLIST_GUARD
#define GUI2HORIZONTALLIST_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2horizontalList: public GUI2base
{
	public:
	bool wrap;
	GUI2horizontalList();
	void render(void* arg);
	void invalidate(vec2i newPos, vec2i newSize);
};
#endif