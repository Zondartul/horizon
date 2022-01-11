#ifndef GUI2LABEL_GUARD
#define GUI2LABEL_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2label: public GUI2base
{
	public:
	string text;
	GUI2label();
	void render(void* arg);
};
#endif