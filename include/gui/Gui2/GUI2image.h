#ifndef GUI2IMAGE_GUARD
#define GUI2FRAME_GUARD
#include "gui/Gui2/GUI2base.h"
#include "resource/textureloader.h"

class GUI2image:public GUI2base
{
	public:
	texture *ImageTex;
	GUI2image();
	void setImage(char *path);
	void render(void *arg);
};
#endif