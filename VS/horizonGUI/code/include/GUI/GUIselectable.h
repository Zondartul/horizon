#ifndef GUISELECTABLE_GUARD
#define GUISELECTABLE_GUARD
class GUIselectable;
class GUIselectionGroup:public GUIbase{
	public:
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