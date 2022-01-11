#ifndef GUI4BASE_GUARD
#define GUI4BASE_GUARD
#include "util/globals.h"
#include "util/functor.h"
#include "input/input.h"
enum GUI4orderType{ORDER_NONE, ORDER_HORIZONTAL, ORDER_VERTICAL};
enum GUI4direction{DIR_NONE, DIR_LEFT, DIR_RIGHT, DIR_UP, DIR_DOWN};

class GUI4base{
	public:
	//variables
	rect area; //position and size
	rect client_area; //area reserved for client (not owned) elements 
	GUI4base *parent; //if null, this object is root in a GUI system.
	vector<GUI4base*> children;
	GUI4orderType client_order;
	bool size_to_contents;
	GUI4direction anchor_point1;
	GUI4direction anchor_point2;
	bool is_visible;
	bool is_clickable;
	bool is_resizible;
	bool is_movable;
	bool invalidate_suppressed;
	bool is_client;
	vec2i click_pos;
	vec2i prev_win_pos;
	int click_state;
	//functions
	GUI4base(); //constructor
	~GUI4base(); //destructor
	//coordinate conversion
	vec2i ParentToWindow(vec2i A);	//parent-relative becomes this-relative
	vec2i WorldToWindow(vec2i A);	//absolute becomes this-relative
	vec2i WindowToClient(vec2i A);	//this-relative becomes client area-relative
	vec2i ClientToWindow(vec2i A);
	vec2i WindowToWorld(vec2i A);
	vec2i WindowToParent(vec2i A);
	rect getVisibleWorldArea();
	//mutators
	GUI4base &setPos(int x, int y);					//new position
	GUI4base &setSize(int w, int h);				//new size
	GUI4base &setClientPos(int x, int y);			//change origin of client area
	GUI4base &setClientSize(int w, int h);			//change size of cleint area
	GUI4base &setClientOrder(GUI4orderType order);	//use this order now and every time object is invalidated
	GUI4base &setSizeToContents(bool value);		//resize object to fit contents now and every time it is invalidated
	GUI4base &sizeToContents(bool value);			//*** i'm not sure what this does?
	GUI4base &sortContents(bool (*compare)(GUI4base *A, GUI4base *B)); //sort the contents, this time only
	GUI4base &setAnchor(GUI4direction point1, GUI4direction point2); //make the edge of the object follow the edge of the parent's client area
	GUI4base &snapToSide(GUI4direction direction);			//move the object to the edge of the parent's client area
	GUI4base &setVisible(bool value);				//make object visible or not
	GUI4base &setClickable(bool value);				//the object will receive mouse click events
	GUI4base &setClickThrough(bool value);			//clicking an object also clicks what's below it
	GUI4base &setResizible(bool value);				//the object can be resized by dragging it's borders
	GUI4base &setMovable(bool value);				//the object can be moved by clicking on it and dragging
	GUI4base &setClient(bool value);				//the object is a client and not part of a composite element.
	GUI4base &addElement(GUI4base *A);				//add a child element
	//signals
	void close(); 			//delete the object
	void invalidate(); 		//recalculate all positioning data
	void invalidateSelf(); 	//recalculate personal positioning data only
	void bringToFront(); 	//make this the topmost control within it's neighborhood
	virtual void render(); 			//draw the element
	virtual void think();
	//events
	functor *onMouseOver;	//called when mouse moves over the object
	functor *onMouseClick;	//called when mouse clicks the object (if focused)
	functor *onKeyboard;		//called when keyboard button is pressed (if focused)
};
#endif

