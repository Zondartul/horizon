#ifndef GUI2RADIOGROUP_GUARD
#define GUI2RADIOGROUP_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2radiogroup
{
	public:
	void *buttons[32];
	int selection;
	GUI2radiogroup();
	void checkButton(void *btn);
	void addButton(void *btn);
};
#endif