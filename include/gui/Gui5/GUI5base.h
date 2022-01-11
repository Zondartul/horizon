#ifndef GUI5BASE_GUARD
#define GUI5BASE_GUARD
#include "util/messenger.h"
//abstract class: Every element in GUI5 inherits from GUI5base
//but GUI5base can not have instances itself

class GUI5base: public messageReceiver{
	public:
	rect area; 				//position and size of this element.
							//relative to: parent (or world if none)
	rect client_area;		//determines allowed positions for clients.
							//relative to: this->area.
							//can be smaller or larger than this->area.
	rect client_draw_area;	//clients will only be drawn in the section of the
							//screen designated by this rectangle.
							//relative to: this->area.
	
	bool debugmode;			//will display debug information instead of normal render function
	bool mouseover;			//is the mouse cursor currently hovering over the element?
	
	GUI5base *parent;
	vector<GUI5base*> children;
	MessageChannel channel;
	bool isClient;
	//children are divided into two groups: clients, and non-clients (staff)
	//non-clients:
	//	positioned relative to	this->area
	//	drawn within			this->area
	//clients:
	//	positioned relative to	this->client_area
	//	drawn within			this->client_draw_area
	
	
	GUI5base();							//constructor
	//mutators
	GUI5base &setPos(vec2i pos);		//change position (with side-effects)
	GUI5base &setSize(vec2i size);		//change size	(with side-effects)
	GUI5base &addElement(GUI5base &A);	//add a child element
	GUI5base &setDebug(bool debug);
	GUI5base &setClient(bool client);
	//signals
	virtual void think();						//perform any logic not related to rendering
	virtual void renderlogic();					//perform render/don't render logic and children render logic
	virtual void render();						//draw this element
	virtual void debugrender();					//this will be used instead in debugmode
	virtual void receiveMessage(message *msg);	//react to message, e.g. mouse click
	virtual void invalidate();					//start invalidation process (figure out who to start with)
	virtual void layoutlogic();					//do this/children layout logic
	virtual void layout();						//perform layout of this element only
	virtual void close();						//delete element
};

// various old herp derps:

//invalidation: an event when the position or size of the element or it's
	//children no longer satisfies some constraints. When that happens,
	//this->invalidate() is called in order to fix all positions and sizes.
	//Rules of invalidation:
	//size:
	//	does not depend on own position
	//	does depend on children's position
	//	does depend on children's size
	//	does not depend on neighbor's position
	//	does not depend on neighbor's size
	//	does not depend on parent's position
	//	does depend on parent's size
	//position:
	//	does depend on own size, but in a way known only to parent
	//	does not depend on children's position
	//	does not depend on children's size
	//	does depend on neighbor's position, but in a way known only to parent
	//	does depend on neighbor's size, but in a way known only to parent
	//	does not depend on parent's position
	//	does depend on parent's size
	//
	//Given these rules, let's figure out what happens in the case where
	//A is a parent of B1 and B2
	//B1 is a parent of C1 and C2
	//B2 is a parent of D1 and D2
	//We will make a table to show how changing the position of any one element
	//in this inter-dependent system causes other elements to be invalidated.
	//			invalidates:
	// Changing		A	B1	B2	C1	C2	D1	D2
	//	pos of:
	//	A			1	0	0	0	0	0	0	
	//	B1				1						
	//	B2					1					
	//	C1						1				
	//	C2							1				
	//	D1								1		
	//	D2									1	
	
	// ... it actually invalidates everything. huh. 
	//see http://www.gamedev.net/topic/673813-gui-elements-whose-positions-depend-on-one-another/
	
	//As such, for a case where A is a parent of B and B is a parent of C,
	//the suggested order of invalidation is such:
	//	C calculates own size
	//	B calculates C's position
	//	B calculates own size
	//	A calculates B's position
	//	A calculates own size.
	//External forces may cause the position and/or size of any element in
	//the family tree to change; As the effects 
	
	//---------------------------- OR ------------------------------------------
	// GameDev suggests a Two-Pass system: in a Measure() pass, parent
	// recursively asks children for suggestions about their size,
	// Then in the Assign() pass, parent recursively enforces some size
	// 

#endif