#ifndef GUI2RADIOBUTTON_GUARD
#define GUI2RADIOBUTTON_GUARD
#include "gui/Gui2/GUI2base.h"
#include "gui/Gui2/GUI2radiogroup.h"
class GUI2radiobutton: public GUI2base
{
	public:
	bool checked;
	GUI2radiogroup* group;
	GUI2radiobutton();
	void onClick(int mb);
	void render(void* arg);
};
#endif