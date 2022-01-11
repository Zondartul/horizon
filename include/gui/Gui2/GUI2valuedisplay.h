#ifndef GUI2VALUEDISPLAY_GUARD
#define GUI2VALUEDISPLAY_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2valuedisplay:public GUI2base
{
	public:
	void *val;
	char mode;
	int precision;
	char str[256];
	GUI2valuedisplay();
	void render(void *arg);
};
#endif