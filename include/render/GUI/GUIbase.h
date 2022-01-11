#ifndef GUIBASE_GUARD
#define GUIBASE_GUARD
#include "GUI_internal.h"
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
	virtual ~GUIbase();
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



#endif
