#ifndef GUIFRAME_GUARD
#define GUIFRAME_GUARD
#include "GUI/GUIbase.h"
#include "util/globals_gui.h"

class DLLAPI_GUI GUIframe:public virtual GUIbase{
	public:
	GUIframe();
	vec3 bgColor;
	vec3 borderColor;
	bool noframe;
	GUIframe *setBgColor(vec3 color);
	GUIframe *setBorderColor(vec3 color);
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);
	virtual string getType(); 
};
#endif
