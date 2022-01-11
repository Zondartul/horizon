#ifndef GUI_GUARD
#define GUI_GUARD
#include "vec.h"
#include "event.h"
#include "fonts.h"
#include <functional>
using std::function;

//base GUI class
class GUIbase:public eventListener{
	public:
	GUIbase *parent;
	vector<GUIbase*> children;
	bool isClient;
	bool hidden;
	rect area;
	rect clientArea;
	GUIbase();
	//add new GUI element to this one
	GUIbase &addChild(GUIbase &child);
	
	//set the new dimensions of this element
	//also sets the client area to match.
	GUIbase &setSize(vec2i newSize);
	
	//change the position of this element
	//without altering size
	GUIbase &moveTo(vec2i newStart);
	
	//changes area that other elements are 
	//positioned in relation to and can be drawn
	//in.
	GUIbase &setClientArea(rect newclientarea);
	
	//hides the element and causes it to stop
	//receiving events
	GUIbase &setHidden(bool newHidden);
	
	//resizes the element to fit the contents.
	virtual GUIbase &sizeToContents();
	
	//call this to render the element and all
	//it's children.
	virtual void renderLogic();
	
	//this is called to render just this element.
	virtual void render();
	
	//this is called by the event system.
	virtual void onEvent(eventKind event);
	
	//this is called whenever a sizing or moving event occurs
	virtual void invalidate();
	
	//translates from element-relative to world coords
	vec2i thisToWorld(vec2i L);
	rect thisToWorld(rect L);
	//translates from client-area-relative to world coords.
	vec2i clientToWorld(vec2i L);
	rect clientToWorld(rect L);
	//gets element dimensions in world coords
	rect worldArea();
	//gets client area in world coords
	rect worldClientArea();
	//gets the element's visible area rect in world coords
	rect visibleArea();
	//gets the client's visible area rect in world coords
	rect visibleClientArea();
	
};

class GUIframe:public GUIbase{
	public:
	GUIframe();
	vec3ub bgColor;
	vec3ub borderColor;
	GUIframe &setBgColor(vec3ub color);
	GUIframe &setBorderColor(vec3ub color);
	virtual void render();
};

class GUIlabel:public GUIbase{
	public:
	GUIlabel();
	vec3ub textColor;
	font textfont;
	string text;
	GUIlabel &setTextColor(vec3ub color);
	GUIlabel &setTextFont(font f);
	GUIlabel &setText(string newtext);
	virtual GUIbase &sizeToContents();
	virtual void render();
};

class GUIimage:public GUIframe{
	public:
	GUIimage();
	texture image;
	GUIimage &setImage(texture newimage);
	virtual GUIbase &sizeToContents();
	virtual void render();
};

class GUIbutton:public GUIframe{
	public:
	GUIbutton();
	vec3ub textColor;
	vec3ub hoverColor;
	vec3ub pressedColor;
	bool mouseover;
	bool pressed;
	font textfont;
	string text;
	GUIbutton &setTextColor(vec3ub color);
	GUIbutton &setHoverColor(vec3ub color);
	GUIbutton &setPressedColor(vec3ub color);
	GUIbutton &setTextFont(font newfont);
	GUIbutton &setText(string newtext);
	texture image;
	GUIbutton &setImage(texture newimage);
	function<void()> F;
	GUIbutton &setFunction(function<void()> f);
	virtual GUIbase &sizeToContents();
	virtual void render();
	virtual void onEvent(eventKind event);
};

class GUIscrollbarBar:public GUIframe{
	public:
	vec3ub hoverColor;
	vec3ub pressedColor;
	bool mouseover;
	bool pressed;
	vec2i offset;
	GUIscrollbarBar();
	virtual void render();
	virtual void onEvent(eventKind event);
};

class GUIscrollbar:public GUIframe{
	public:
	vec2i innerSize;
	//vec2i scrollPos;
	bool bsizeToParent;
	bool vertical;
	bool horizontal;
	
	GUIscrollbar();
	GUIscrollbar &setInnerSize(vec2i newInnerSize);
	GUIscrollbar &setScrollOffset(vec2i newScrollOffset);
	GUIscrollbar &setScrollRatio(vec2f newScrollRatio);
	GUIscrollbar &sizeToParent(bool b);
	GUIscrollbar &setVertical(bool newVertical);
	GUIscrollbar &setHorizontal(bool newHorizontal);
	virtual void renderLogic();
	virtual void render();
	virtual void invalidate();
	rect vtrackRect();
	rect htrackRect();
	vec2i trackDimensions();
	vec2f areaRatio();
	vec2f normalizedAreaRatio();
	vec2f scrollRatio();
	vec2i scrollOffset();
};

class GUIwindow:public GUIframe{
	public:
	bool hasCloseButton;
	bool moveable;
	bool pressed;
	bool mouseover;
	vec2i offset;
	GUIwindow();
	GUIwindow &setCloseButton(bool hasbtn);
	GUIwindow &setMoveable(bool newmoveable);
	GUIwindow &setTitle(string title);
	void close();
	virtual void render();
	virtual void onEvent(eventKind event);
	virtual void invalidate();
};
#endif