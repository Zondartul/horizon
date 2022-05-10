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
	
	virtual string getType(); 
};

class GUIscrollbar:public virtual GUIframe{
	public:
	
	vec2 innerSize;
	bool bSizeToParent;
	bool vertical;
	bool horizontal;
    bool scrollingEnabled;
	bool constructed = false;
	
	vec2 offset;
	vec2 prevoffset;

	GUIscrollbar();
	GUIbutton *btnUp;		
	GUIbutton *btnDown;		
	GUIscrollbarBar *vbar;	
	GUIbutton *btnLeft;		
	GUIbutton *btnRight;	
	GUIscrollbarBar *hbar;	
	GUIframe *inner;		
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

	void clampInnerSizeToOuter(); 
	rect vtrackRect();
	rect htrackRect();
	vec2 trackDimensions();
	vec2 areaRatio();
	vec2 scrollRatio();
	vec2 scrollOffset();
	virtual void onEvent(eventKind event);
	
	
	virtual GUIpropertyTable getDefaultPropertyTable();
	virtual string getProperty(string key);
	virtual void setProperty(string key, string val);

	virtual string getType(); 
	
};


#endif 
