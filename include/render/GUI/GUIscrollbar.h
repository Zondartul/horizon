#ifndef GUISCROLLBAR_GUARD
#define GUISCROLLBAR_GUARD
#include "GUIframe.h"
#include "GUIbutton.h"

class GUIscrollbarBar:public virtual GUIframe{
	public:
	vec3 hoverColor;
	vec3 pressedColor;
	//bool mouseover;
	bool pressed;
	vec2 offset;
	GUIscrollbarBar();
	virtual void render();
	virtual void onEvent(eventKind event);
};

class GUIscrollbar:public virtual GUIframe{
	public:
	vec2 innerSize;
	//vec2 scrollPos;
	bool bsizeToParent;
	bool vertical;
	bool horizontal;
	vec2 offset;
	vec2 prevoffset;
    bool scrollingEnabled;
    map<GUIbase*, vec2> initialOffsets;

	GUIscrollbar();
	GUIscrollbar *setInnerSize(vec2 newInnerSize);
	GUIscrollbar *setScrollOffset(vec2 newScrollOffset);
	GUIscrollbar *setScrollRatio(vec2 newScrollRatio);
	GUIscrollbar *sizeToParent(bool b);
	GUIscrollbar *setVertical(bool newVertical);
	GUIscrollbar *setHorizontal(bool newHorizontal);
    GUIscrollbar *disableScrolling();
    GUIscrollbar *enableScrolling();
	virtual void renderLogic();
	virtual void render();
	virtual void invalidate();
	virtual GUIbase *addChild(GUIbase *child);

	rect vtrackRect();
	rect htrackRect();
	vec2 trackDimensions();
	vec2 areaRatio();
	//vec2 normalizedAreaRatio();
	vec2 scrollRatio();
	vec2 scrollOffset();
	virtual void onEvent(eventKind event);
};


#endif // GUISCROLLBAR_GUARD
