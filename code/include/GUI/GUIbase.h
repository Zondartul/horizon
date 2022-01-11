#ifndef GUIBASE_GUARD
#define GUIBASE_GUARD

//Base GUI class. Handles 90% of common GUI logic. All GUI widgets are derivatives
//of this class.
//Inherits from eventListener to accept events,
//and from elastic_ptr_anchor to accept elastic pointers.

class GUIbase:public eventListener, public elastic_ptr_anchor{
	public:
	//======= MEMBER VARIABLES ====================
	//----- essential variables --------
	//every widget has a unique name. Widget can be found in a tree by name.
	string name;	
	//most widgets have a parent. The parent dispences all events and 
	//calls for rendering, ticks, and mouseover calculation.
	GUIbase *parent;
	//widgets can have other widgets as children. Those children can either be
	//parts of the widget (non-clients) or independent widgets (clients).
	vector<GUIbase*> children;
	typedef typename vector<GUIbase*>::iterator ChI;
	//the events (key presses, mouse moves, mouse clicks, frame syncs) are
	//passed from parent to children through this event channel.
	eventChannel clientChannel; 
	eventChannel partChannel;
	//------ widget geometry -------
	//the rect that defines the position and size of this widget relative
	//to parent.
	rect area;
	//the client area is where client child widgets will be drawn.
	rect clientArea;
	//if this widget is a client (default), it will be positioned relative to the
	//client area of the parent widget. If the widget is not a client, but part
	//of the parent widget, it will be positioned relative the main area of the
	//parent widget.
	bool isClient;		 
	//-------state ----------------
	//a widget has "mouseover" if the mouse cursor is within the area of the
	//widget. If the widget has mouseover, then it's ancestors likely also
	//have mouseover.
	bool mouseover;	
	//when the user does something to the widget that can change its layout,
	//invalidate() is called. The widget can set this flag to prevent invalidate()
	//from being called, e.g. inside invalidate() or when automatically changing
	//something from within the widget.
	bool suppressInvalidate;
	//a widget can be hidden. The widget still exists, but is not visible
	//and not interactable. This is an alternative to destroying and creating
	//the same widgets over and over.
	bool hidden;	
	//if this flag is set, the child elements cannot be interacted with and
	//behave as if they were behind this element.
	bool blockChildInput;
	//if this flag is set, the element will be deleted on the next cleanup.
	//deletion is deferred until all GUI object member function calls are
	//definitely finished.
	bool deletePending;
	//------ render --------------
	//a widget's appearance is handled entirely by the renderer.
	GUIrenderer *renderer;
	//if this flag is set, the object will be rendered using high-visibility,
	//verbose debug view (usually, colored wireframe).
	bool render_debug;	   
	//if this flag is set, the object will be rendered without scissoring, i.e.
	//visible even outside of the area allowed by it's parent.
	bool render_noscissor; 
	
	//====== METHODS =========================
	//-------- GUI building --------------------
	//constructor / destructor.
	GUIbase();
	virtual ~GUIbase(); 
	//add new GUI element to this one
	virtual GUIbase *addChild(GUIbase *child);
	
	//add a new GUI element at a position
	virtual ChI addChild(GUIbase *child, ChI iter);

	//remove an existing element cleanly
	virtual GUIbase *removeChild(GUIbase *child);

	//move the child to the back of the children list,
	//as if it were the last one added.
	virtual GUIbase *moveChildToTheTop(GUIbase *child);

	//switch a child between part and client modes
	//parts (non-clients) receive events from a different,
	//unfiltered channel.
	virtual GUIbase *makePart(GUIbase *child);
	virtual GUIbase *makeClient(GUIbase *child);
	//------- Layout changes ----------------------
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

	//------------- enumeration -------------------
	//returns the ancestor without a parent.
	GUIbase *root();
	
	//returns the lowest-level child that has mouseover.
	GUIbase *getMouseoverElement();

	//returns true if this object is somewhere along the line of parents
	//of the child.
	bool isAncestorOf(GUIbase *child);

	//returns true if this object is the parent of the child.
	bool isParentOf(GUIbase *child);
	
	//searches among the descendants and returns the first element
	//with matching name.
	GUIbase *getByName(string name);

	//same but matches by getType() instead.
	GUIbase *getByType(string type);
	
	//helper function for the above two
	GUIbase *getByHelper(string askname, string asktype);
	
	//------------ event processing ---------------
	//calls tick on all children.
	virtual void tickLogic();
	
	//time-dependent logic. Called several times a second.
	virtual void tick();
	
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

	//perform the mouseover check for all the widgets in the tree.
	virtual void recalcMouseoverTree();

	//mark object for deletion
	virtual void close();
	
	//recursively check if any widgets want to be deleted,
	//delete if necessary.
	virtual void checkCloseTree();
	
	//----------- geometry calculation --------------------
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
	//calculates the set of rects, one for each border and corner.
	GUI_border_rects getBorders(GUI_border_size border = GUI_border_size());
	//tests each of the border rects for containing the specified point.
	GUIe_border testBorders(vec2 pos, GUI_border_size border =  GUI_border_size());
	
	//----------- property tables --------------------------
	//Property tables are string-string dictionaries that are used to store
	//the main properties of a widget for serialization.
	//Compound properties are hierachical structures that contain property tables
	//for all the widgets in a GUI tree.
	
	//returns the string with the name of the most-derived-class of the widget.
	virtual string getType();
	//returns a property table for this widget only.
	virtual GUIpropertyTable getPropertyTable();
	//sets the properties of this widget to those described in the table.
	virtual void setPropertyTable(GUIpropertyTable table);
	
	//returns the table of default properties for this widget.
	virtual GUIpropertyTable getDefaultPropertyTable();
	//returns, as a string, the current value of the property by a given name.
	virtual string getProperty(string key); 
	//sets the given property to a new value parsed from string.
	virtual void setProperty(string key, string val);
	//returns a compound property describing this widget and all of its children.
	virtual GUIcompoundProperty getCompoundProperty();
	//sets the properties of this widget and all children to match the given
	//compound property. Referenced children are created if they do not exist.
	virtual void setCompoundProperty(const GUIcompoundProperty prop);
};



#endif
