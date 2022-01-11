#ifndef GUI5BUTTON_GUARD
#define GUI5BUTTON_GUARD
#include "gui/Gui5/GUI5base.h"
#include "util/globals.h"

class GUI5button:public GUI5base{
	public:
	string text;
	bool pressed;
	GUI5button();
	GUI5button &setText(string text);
	GUI5button &setFunc(funcptr f);
	GUI5button &setArg(void *a);
	funcptr func;
	void *arg;
	void receiveMessage(message *msg);
	void render();
	void layout();
};

#endif