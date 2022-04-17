#ifndef GUIFRAME_GUARD
#define GUIFRAME_GUARD
#include "GUIbase.h"

class GUIframe:public virtual GUIbase{
	public:
	GUIframe();
	vec3 bgColor;
	vec3 borderColor;
	bool noframe;
	GUIframe *setBgColor(vec3 color);
	GUIframe *setBorderColor(vec3 color);
	
	//property table
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); //returns the most-derived-class of the widget.
};

#endif
