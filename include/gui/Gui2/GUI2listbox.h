#ifndef GUI2LISTBOX_GUARD
#define GUI2LISTBOX_GUARD
#include "gui/Gui2/GUI2base.h"
#include "gui/Gui2/GUI2button.h"
class GUI2listbox: public GUI2base
{
	public:
	GUI2button* sel[32];
	int selected;
	string selText;
	void (*callback)(void*);
	void* callarg;
	GUI2listbox();
	static void wrapFunc(void* arg);
	void addOption(string text, void (*func)(void*), void* arg);
	void render(void* arg);
	
	void invalidate(vec2i newPos, vec2i newSize);
};
#endif