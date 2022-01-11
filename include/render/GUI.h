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
	eventChannel channel;
	bool isClient;
	bool hidden;
	bool mouseover;
	bool suppressInvalidate;
	rect area;
	rect clientArea;
	GUIbase();
	~GUIbase();
	//add new GUI element to this one
	virtual GUIbase *addChild(GUIbase *child);
	
	//remove an existing element cleanly
	virtual GUIbase *removeChild(GUIbase *child);
	
	//set the new dimensions of this element
	//also sets the client area to match.
	virtual GUIbase *setSize(vec2 newSize);
	
	//change the position of this element
	//without altering size
	virtual GUIbase *moveTo(vec2 newStart);
	
	//changes area that other elements are 
	//positioned in relation to and can be drawn
	//in.
	virtual GUIbase *setClientArea(rect newclientarea);
	
	//hides the element and causes it to stop
	//receiving events
	virtual GUIbase *setHidden(bool newHidden);
	
	//resizes the element to fit the contents.
	virtual GUIbase *sizeToContents();
	
	//returns the ancestor without a parent.
	GUIbase *root();
	
	//call this to render the element and all
	//it's children.
	virtual void renderLogic();
	
	//this is called to render just this element.
	virtual void render();
	
	//this is called by the event system.
	virtual void onEvent(eventKind event);
	
	//recalculate element geometry
	virtual void invalidate();
	
	//invalidate the entire GUI tree
	virtual void invalidateTree();
	
	//invalidate the element and it's children
	virtual void invalidateDown();
	
	//check if the mouse is over this element (including subelements)
	virtual void recalcMouseover();
	
	//translates from element-relative to world coords
	virtual vec2 thisToWorld(vec2 L);
	virtual rect thisToWorld(rect L);
	//translates from client-area-relative to world coords.
	virtual vec2 clientToWorld(vec2 L);
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

struct gridcell{
	int spanx;
	int spany;
	GUIbase *child;
};
typedef vector<gridcell> gridrow;
typedef vector<gridrow> gridkind;

struct gridline{
	float min;		//minimum size
	float max;		//maximum size
	float weight;	//how much of the extra space to give this line
	float cur;		//current size of grid line (set automatically)
	float pos;		//current position of grid line (set automatically)
};

struct linespan{
	int from;
	int to;
	gridcell *cell;
};

class GUIgrid:public virtual GUIbase{
	public:
	GUIgrid();
	virtual void grid(GUIbase *child, int row = -1, int col = 0, int spanx = 1, int spany = 1);
	virtual void configureRow(int row, float min, float max, float weight);
	virtual void configureColumn(int col, float min, float max, float weight);
	void resize(int numrows, int numcols);
	void clear();
	gridkind rows;
	vector<gridline> rowsettings;
	vector<gridline> colsettings;
	vector<linespan> rowspans;
	vector<linespan> colspans;
	virtual void render();
	virtual void invalidate();
};

class GUIframe:public virtual GUIbase{
	public:
	GUIframe();
	vec3 bgColor;
	vec3 borderColor;
	GUIframe *setBgColor(vec3 color);
	GUIframe *setBorderColor(vec3 color);
	virtual void render();
};

enum alignmentKind{ALIGN_NONE,ALIGN_LEFT,ALIGN_RIGHT,ALIGN_TOP,ALIGN_BOTTOM,ALIGN_CENTER};

class GUIlabel:public virtual GUIbase{
	public:
	GUIlabel();
	vec3 textColor;
	font *textfont;
	string text;
	alignmentKind alignment_horizontal;
	alignmentKind alignment_vertical;
	bool const_height;	//1 - text height depends on font, 0 - text height depends on actual text
	GUIlabel *setTextColor(vec3 color);
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
	//vec3 textColor;
	vec3 hoverColor;
	vec3 pressedColor;
	//bool mouseover;
	bool pressed;
	//font *textfont;
	//string text;
	//GUIbutton *setTextColor(vec3 color);
	//GUIbutton *setHoverColor(vec3 color);
	//GUIbutton *setPressedColor(vec3 color);
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
	
	GUIscrollbar();
	GUIscrollbar *setInnerSize(vec2 newInnerSize);
	GUIscrollbar *setScrollOffset(vec2 newScrollOffset);
	GUIscrollbar *setScrollRatio(vec2 newScrollRatio);
	GUIscrollbar *sizeToParent(bool b);
	GUIscrollbar *setVertical(bool newVertical);
	GUIscrollbar *setHorizontal(bool newHorizontal);
	virtual void renderLogic();
	virtual void render();
	virtual void invalidate();
	rect vtrackRect();
	rect htrackRect();
	vec2 trackDimensions();
	vec2 areaRatio();
	vec2 normalizedAreaRatio();
	vec2 scrollRatio();
	vec2 scrollOffset();
};

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