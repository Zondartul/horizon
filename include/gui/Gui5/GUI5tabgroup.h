#ifndef GUI5TABGROUP_GUARD
#define GUI5TABGROUP_GUARD
#include "gui/Gui5/GUI5base.h"

//displays each child in a separate, selectable "tab"
class GUI5tabgroup:public GUI5base{
	public:
	int currentTab;
	map<int, string> titles;
	GUI5tabgroup();
	GUI5tabgroup &setTitle(int n, string title);
	void render();
	void layout();
};
#endif