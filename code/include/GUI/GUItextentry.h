#ifndef GUITEXTENTRY_GUARD
#define GUITEXTENTRY_GUARD

class GUItextEntry:public virtual GUIbutton{
	public:
	
	vec3 focusedColor;
	bool callOnEdit;
	bool callOnEnter;
	bool callOnUnfocus;
	bool unfocusOnEnter;
	bool clearOnEnter;
	bool multiline;
	bool numeric;
	texture *tcaret;
	
	bool hasfocus;
	unsigned int cursorPos;
	GUItextEntry();
	GUItextEntry *setText(string newtext);
	GUItextEntry *setFunction(function<void()> f);
	double getNumber();
	virtual void onEvent(eventKind event);
	
	
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); 
};

#endif
