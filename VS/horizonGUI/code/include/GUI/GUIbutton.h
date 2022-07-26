#ifndef GUIBUTTON_GUARD
#define GUIBUTTON_GUARD
#include "util/globals_gui.h"

class DLLAPI_GUI GUIbutton:
	public virtual GUIframe, 
	public virtual GUIimage, 
	public virtual GUIlabel
{
	public:
	GUIbutton();
	vec3 hoverColor;
	vec3 pressedColor;
	bool pressed;
	float lastPressTime;
	function<void()> F;
	GUIbutton *setFunction(function<void()> f);
	virtual GUIbase *sizeToContents();
	virtual void onEvent(eventKind event);
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);
	virtual string getType(); 
};

#endif 
