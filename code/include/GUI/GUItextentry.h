#ifndef GUITEXTENTRY_GUARD
#define GUITEXTENTRY_GUARD
//#include "GUIbutton.h"

class GUItextEntry:public virtual GUIbutton{
	public:
	//properties
	vec3 focusedColor;
	bool callOnEdit;
	bool callOnEnter;
	bool callOnUnfocus;
	bool unfocusOnEnter;
	bool clearOnEnter;
	bool multiline;
	bool numeric;
	texture *tcaret;
	//internal vars
	bool hasfocus;
	int cursorPos;
	//string text;
	//function<void()> F;
	GUItextEntry();
	GUItextEntry *setText(string newtext);
	//GUItextEntry *setCallOnEdit(bool b);
	//GUItextEntry *setCallOnEnter(bool b);
	//GUItextEntry *setMultiline(bool b);
	//GUItextEntry *setNumeric(bool b);
	GUItextEntry *setFunction(function<void()> f);
	double getNumber();
	//virtual void render();
	virtual void onEvent(eventKind event);
	
	//property table
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); //returns the most-derived-class of the widget.
};

#endif
