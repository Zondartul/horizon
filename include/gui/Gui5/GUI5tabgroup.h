#ifndef GUI5TABGROUP_GUARD
#define GUI5TABGROUP_GUARD
#include "gui/Gui5/GUI5base.h"

//used internally
struct tab{
	string title;
	vec2i p1;	//rendering coords
	vec2i p2;
	vec2i p3;
	vec2i p4;
	vec2i p5;
};

//displays each child in a separate, selectable "tab"
class GUI5tabgroup:public GUI5base{
	public:
	int currentTab;
	map<int, tab> tabs;
	GUI5tabgroup();
	GUI5tabgroup &setTitle(int n, string title);
	void receiveMessageExtra(message *msg);
	void render();
	void layout();
};
#endif