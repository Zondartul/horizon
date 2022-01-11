#ifndef GUI2TEXTENTRY_GUARD
#define GUI2TEXTENTRY_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2textEntry: public GUI2base
{
	public:
	string text;
	bool multiline;
	bool sizeToContents;
	void (*callback)(void*);
	void* arg;
	GUI2textEntry();
	void render(void* arg);
	void onKeyboard(string kb);
};
#endif