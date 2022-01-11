#ifndef GUI5IMAGE_GUARD
#define GUI5IMAGE_GUARD
#include "gui/Gui5/GUI5base.h"
#include "resource/textureloader.h"

class GUI5image:public GUI5base{
	public:
	texture *tex;
	GUI5image &setImage(string img);
	void render();
	void layout();
};

#endif