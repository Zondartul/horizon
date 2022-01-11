#ifndef GUI2SCROLLSLIDEY_GUARD
#define GUI2SCROLLSLIDEY_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2scrollslidey:public GUI2base
{
	public:
	void (*callback)(void*);
	void* arg;
	bool vertical;
	GUI2scrollslidey();
	void onClick(int mb);
	void dragCheck();
	void moveupdown(int dist);
	void render(void *arg);
};
#endif