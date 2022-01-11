#ifndef GUIFRAME_GUARD
#define GUIFRAME_GUARD
#include "GUIbase.h"
//#include "GUI.h"

class GUIframe:public virtual GUIbase{
	public:
	GUIframe();
	vec3 bgColor;
	vec3 borderColor;
	bool noframe;
	GUIframe *setBgColor(vec3 color);
	GUIframe *setBorderColor(vec3 color);
	virtual void render();
};

#endif
