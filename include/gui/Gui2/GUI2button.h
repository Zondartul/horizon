#ifndef GUI2BUTTON_GUARD
#define GUI2BUTTON_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2button: public GUI2base
{
	public:
	void (*func)(void*);
	void* arg;
	string text;
	GLuint image;
	GUI2button();
	void onClick(int mb);
	void setImage(string path);
	void render(void *arg);
};
#endif