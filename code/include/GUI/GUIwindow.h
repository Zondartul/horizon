#ifndef GUIWINDOW_GUARD
#define GUIWINDOW_GUARD
//#include "GUI.h"

class GUIwindow:public virtual GUIframe{
	public:
	//properties
	bool hasCloseButton;
	bool moveable;
	string title;
	//child elements
	GUIlabel *lblTitle;
	GUIbutton *btnClose;
	//grab support
	bool pressed;
	bool mouseovertitle;
	vec2 offset;
	GUIwindow();
	GUIwindow *setCloseButton(bool hasbtn);
	GUIwindow *setMoveable(bool newmoveable);
	GUIwindow *setTitle(string title);
	
	//standard functions
	virtual void onEvent(eventKind event);
	virtual void invalidate();
	
	//property table
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); //returns the most-derived-class of the widget.
};


#endif
