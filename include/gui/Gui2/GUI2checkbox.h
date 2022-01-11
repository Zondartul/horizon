#ifndef GUI2CHECKBOX_GUARD
#define GUI2CHECKBOX_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2checkbox: public GUI2base
{
	public:
	bool checked;
	void (*func)(void*);
	GUI2checkbox();
	
	void onClick(int mb);
	void render(void* arg);
};
#endif