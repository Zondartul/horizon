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
	virtual GUIbase *addChild(GUIbase *child);
	
	//set the new dimensions of this element
	//also sets the client area to match.
	virtual GUIbase *setSize(vec2i newSize);
	
	//change the position of this element
	//without altering size
	virtual GUIbase *moveTo(vec2i newStart);
	
	//changes area that other elements are 
	//positioned in relation to and can be drawn
	//in.
	virtual GUIbase *setClientArea(rect newclientarea);
	
	//hides the element and causes it to stop
	//receiving events
	virtual GUIbase *setHidden(bool newHidden);
	
	//resizes the element to fit the contents.
	virtual GUIbase *sizeToContents();
	
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
	virtual vec2i thisToWorld(vec2i L);
	virtual rect thisToWorld(rect L);
	//translates from client-area-relative to world coords.
	virtual vec2i clientToWorld(vec2i L);
	virtual rect clientToWorld(rect L);
	//gets element dimensions in world coords
	virtual rect worldArea();
	//gets client area in world coords
	virtual rect worldClientArea();
	//gets the element's visible area rect in world coords
	virtual rect visibleArea();
	//gets the client's visible area rect in world coords
	virtual rect visibleClientArea();
	//returns the ancestor with no parent.
	//virtual GUIbase *getRoot();
};

class GUIframe:public virtual GUIbase{
	public:
	GUIframe();
	vec3f bgColor;
	vec3f borderColor;
	GUIframe *setBgColor(vec3f color);
	GUIframe *setBorderColor(vec3f color);
	virtual void render();
};

enum alignmentKind{ALIGN_NONE,ALIGN_LEFT,ALIGN_RIGHT,ALIGN_TOP,ALIGN_BOTTOM,ALIGN_CENTER};

class GUIlabel:public virtual GUIbase{
	public:
	GUIlabel();
	vec3f textColor;
	font *textfont;
	string text;
	alignmentKind alignment_horizontal;
	alignmentKind alignment_vertical;
	bool const_height;	//1 - text height depends on font, 0 - text height depends on actual text
	GUIlabel *setTextColor(vec3f color);
	GUIlabel *setTextFont(font *f);
	GUIlabel *setText(string newtext);
	virtual GUIbase *sizeToContents();
	virtual void render();
};

class GUIimage:public virtual GUIframe{
	public:
	GUIimage();
	texture *image;
	GUIimage *setImage(texture *newimage);
	virtual GUIbase *sizeToContents();
	virtual void render();
};

class GUIbutton:public virtual GUIframe, public virtual GUIimage, public virtual GUIlabel{
	public:
	GUIbutton();
	//vec3f textColor;
	vec3f hoverColor;
	vec3f pressedColor;
	bool mouseover;
	bool pressed;
	//font *textfont;
	//string text;
	//GUIbutton *setTextColor(vec3f color);
	//GUIbutton *setHoverColor(vec3f color);
	//GUIbutton *setPressedColor(vec3f color);
	//GUIbutton *setTextFont(font *newfont);
	//GUIbutton *setText(string newtext);
	//texture *image;
	//GUIbutton *setImage(texture *newimage);
	function<void()> F;
	GUIbutton *setFunction(function<void()> f);
	virtual GUIbase *sizeToContents();
	virtual void render();
	virtual void onEvent(eventKind event);
};

class GUIscrollbarBar:public virtual GUIframe{
	public:
	vec3f hoverColor;
	vec3f pressedColor;
	bool mouseover;
	bool pressed;
	vec2i offset;
	GUIscrollbarBar();
	virtual void render();
	virtual void onEvent(eventKind event);
};

class GUIscrollbar:public virtual GUIframe{
	public:
	vec2i innerSize;
	//vec2i scrollPos;
	bool bsizeToParent;
	bool vertical;
	bool horizontal;
	
	GUIscrollbar();
	GUIscrollbar *setInnerSize(vec2i newInnerSize);
	GUIscrollbar *setScrollOffset(vec2i newScrollOffset);
	GUIscrollbar *setScrollRatio(vec2f newScrollRatio);
	GUIscrollbar *sizeToParent(bool b);
	GUIscrollbar *setVertical(bool newVertical);
	GUIscrollbar *setHorizontal(bool newHorizontal);
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

class GUIwindow:public virtual GUIframe{
	public:
	bool hasCloseButton;
	bool moveable;
	bool pressed;
	bool mouseover;
	vec2i offset;
	GUIwindow();
	GUIwindow *setCloseButton(bool hasbtn);
	GUIwindow *setMoveable(bool newmoveable);
	GUIwindow *setTitle(string title);
	void close();
	virtual void render();
	virtual void onEvent(eventKind event);
	virtual void invalidate();
};

class GUItextEntry:public virtual GUIbutton{
	public:
	vec3f focusedColor;
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