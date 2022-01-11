#ifndef GUIWINDOW_GUARD
#define GUIWINDOW_GUARD
#include "GUI.h"

class GUIwindow:public virtual GUIframe{
	public:
	bool hasCloseButton;
	bool moveable;
	bool pressed;
	//bool mouseover;
	bool mouseovertitle;
	vec2 offset;
	GUIwindow();
	GUIwindow *setCloseButton(bool hasbtn);
	GUIwindow *setMoveable(bool newmoveable);
	GUIwindow *setTitle(string title);
	void close();
	virtual void render();
	virtual void onEvent(eventKind event);
	virtual void invalidate();
};


#endif
