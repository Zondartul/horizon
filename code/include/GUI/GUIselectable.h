#ifndef GUISELECTABLE_GUARD
#define GUISELECTABLE_GUARD

// A selectable item can be clicked. Child elements do not receive the clicks.
// Single click selects the item. Double-click activates the item. 
// Ctrl-click adds an item to the selection. Shift click selects all items in a range. 
// Item selection can be dragged.

class GUIselectable;

//a selection group is a grouping of selactables
//within a form, some of which may be selected.
//acts like a radio-button-group, but for selectables.
class GUIselectionGroup:public GUIbase{
	public:
	//vector<GUIselectable*> selection;
	bool bSizeToParent = false;
	void onEvent(eventKind event);
	virtual string getType();
};

class GUIselectable:public GUIframe{
	public:
	GUIselectable();
	GUIselectionGroup *group;
	bool moveable = true;
	bool selected;
	void onEvent(eventKind event);
	void select();
	
	bool pressed;
	vec2 offset;
	virtual string getType();
};


#endif