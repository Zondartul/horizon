#ifndef GUIIMAGE_GUARD
#define GUIIMAGE_GUARD
#include "GUIframe.h"

class GUIimage:public virtual GUIframe{
	public:
	GUIimage();
	texture *image;
	GUIimage *setImage(texture *newimage);
	virtual GUIbase *sizeToContents();
	virtual void render();
};

#endif // GUIIMAGE_GUARD

