#ifndef GUIBUTTON_GUARD
#define GUIBUTTON_GUARD
//#include "GUIframe.h"
//#include "GUIimage.h"
//#include "GUIlabel.h"

class GUIbutton:public virtual GUIframe, public virtual GUIimage, public virtual GUIlabel{
	public:
	GUIbutton();
	//properties
	vec3 hoverColor;
	vec3 pressedColor;
	//internal vars
	bool pressed;
	float lastPressTime;
	
	function<void()> F;
	GUIbutton *setFunction(function<void()> f);
	virtual GUIbase *sizeToContents();
	//virtual void render();
	virtual void onEvent(eventKind event);
	
	//property table
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); //returns the most-derived-class of the widget.
	
};

#endif // GUIBUTTON_GUARD
