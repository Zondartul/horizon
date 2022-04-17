#ifndef GUISCROLLBAR_GUARD
#define GUISCROLLBAR_GUARD

class GUIscrollbar;

class GUIscrollbarBar:public virtual GUIframe{
	public:
	GUIscrollbar *scroll;
	vec3 hoverColor;
	vec3 pressedColor;
	bool pressed;
	vec2 offset;
	GUIscrollbarBar();
	virtual void onEvent(eventKind event);
	
	virtual string getType(); //returns the most-derived-class of the widget.
};

class GUIscrollbar:public virtual GUIframe{
	public:
	//properties
	vec2 innerSize;
	bool bSizeToParent;
	bool vertical;
	bool horizontal;
    bool scrollingEnabled;
	bool constructed = false;
	//internal vars
	vec2 offset;
	vec2 prevoffset;

	GUIscrollbar();
	GUIbutton *btnUp;		//child 0
	GUIbutton *btnDown;		//child 1
	GUIscrollbarBar *vbar;	//child 2
	GUIbutton *btnLeft;		//child 3
	GUIbutton *btnRight;	//child 4
	GUIscrollbarBar *hbar;	//child 5
	GUIframe *inner;		//child 6
	GUIscrollbar *setSize(vec2 newSize);
	GUIscrollbar *setInnerSize(vec2 newInnerSize);
	GUIscrollbar *sizeToContents();
	GUIscrollbar *setScrollOffset(vec2 newScrollOffset);
	GUIscrollbar *setScrollRatio(vec2 newScrollRatio);
	GUIscrollbar *sizeToParent(bool b);
	GUIscrollbar *setVertical(bool newVertical);
	GUIscrollbar *setHorizontal(bool newHorizontal);
    GUIscrollbar *disableScrolling();
    GUIscrollbar *enableScrolling();
	
	virtual void renderLogic();
	virtual void invalidate();
	virtual GUIbase *addChild(GUIbase *child);

	void clampInnerSizeToOuter(); //if innerSize < size, innerSize = size
	rect vtrackRect();
	rect htrackRect();
	vec2 trackDimensions();
	vec2 areaRatio();
	vec2 scrollRatio();
	vec2 scrollOffset();
	virtual void onEvent(eventKind event);
	
	//property table
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); //returns the most-derived-class of the widget.
	
};


#endif // GUISCROLLBAR_GUARD
