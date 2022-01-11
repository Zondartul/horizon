#ifndef GUI2IMAGE_GUARD
#define GUI2FRAME_GUARD
#include "gui/Gui2/GUI2base.h"
class GUI2image:public GUI2base
{
	public:
	GLuint ImageTex;
	GUI2image();
	void setImage(char *path);
	void render(void *arg);
};
#endif