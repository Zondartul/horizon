#ifndef GUI2DROPDOWNLIST_GUARD
#define GUI2DROPDOWNLIST_GUARD
#include "gui/Gui2/GUI2base.h"
#include "gui/Gui2/GUI2listbox.h"
class GUI2dropdownlist: public GUI2base
{
	public:
	GUI2listbox* list;
	GUI2button* btn;
	string text;
	bool open;
	static void showList(void* arg);
	static void setCurOption(void* arg);
	GUI2dropdownlist();
	void addOption(string text, void (*func)(void*), void* arg);
	void render(void* arg);
	void invalidate(vec2i newPos, vec2i newSize);
};
#endif