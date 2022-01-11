#ifndef GUI2FRAME_GUARD
#define GUI2FRAME_GUARD
#include "gui/Gui2/GUI2base.h"
#include "gui/Gui2/GUI2button.h"
class GUI2frame: public GUI2base
{
	public:
	string title;
	GUI2button* CloseButton;
	static void btnClose(void* arg);
	GUI2frame();
	void invalidate(vec2i newPos, vec2i newSize);
	
	
	void recalculateClientRect();
	void render(void* arg);
};
#endif