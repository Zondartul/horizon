#ifndef GUITEXTENTRY_GUARD
#define GUITEXTENTRY_GUARD
#include "util/globals_gui.h"

class DLLAPI_GUI GUItextEntry:public virtual GUIbutton{
	public:
	//signals
	bool callOnEdit;
	bool callOnEnter;
	bool callOnUnfocus;
	GUItextEntry* setFunction(function<void()> f);
	// behavior
	bool unfocusOnEnter;
	bool clearOnEnter;
	bool multiline;
	bool numeric;
	//style
	vec3 focusedColor;
	texture *tcaret;
	//state --------------
	bool hasfocus;
	unsigned int cursorPos;
	void caret_left(bool selecting); // moves caret by one character
	void caret_right(bool selecting);
	//----- end state ----
	//selection ----------
	unsigned int select_begin;
	unsigned int select_end;
	bool selecting;

	string get_selection();
	void replace_selection(string rep); // edits the text under selection
	void clear_selection(); // unselects text
	void begin_selection();
	void select_left();  // adds left symbol to selection
	void select_right(); // adds right symbol to selection
	//----- end selection -------
	//data
	GUItextEntry* setText(string newtext);
	double getNumber();
	//base methods
	GUItextEntry();
	virtual void onEvent(eventKind event);
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);
	virtual string getType(); 
};

#endif
