#ifndef GUIWINDOW_GUARD
#define GUIWINDOW_GUARD


class GUIwindow:public virtual GUIframe{
	public:
	
	bool hasCloseButton;
	bool moveable;
	string title;
	
	GUIlabel *lblTitle;
	GUIbutton *btnClose;
	
	bool pressed;
	bool mouseovertitle;
	vec2 offset;
	GUIwindow();
	GUIwindow *setCloseButton(bool hasbtn);
	GUIwindow *setMoveable(bool newmoveable);
	GUIwindow *setTitle(string title);
	
	
	virtual void onEvent(eventKind event);
	virtual void invalidate();
	
	
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); 
};


#endif
