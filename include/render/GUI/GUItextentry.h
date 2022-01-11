#ifndef GUITEXTENTRY_GUARD
#define GUITEXTENTRY_GUARD
#include "GUIbutton.h"

class GUItextEntry:public virtual GUIbutton{
	public:
	vec3 focusedColor;
	bool hasfocus;
	bool callOnEdit;
	bool callOnEnter;
	bool callOnUnfocus;
	bool unfocusOnEnter;
	bool clearOnEnter;
	bool multiline;
	bool numeric;
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
	virtual void render();
	virtual void onEvent(eventKind event);
};

#endif
