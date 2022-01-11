#ifndef GUI5CONSTRAINTMANAGER_GUARD
#define GUI5CONSTRAINTMANAGER_GUARD
#include "util/globals.h"
#include "gui/Gui5/GUI5constraint.h"

class GUI5constraintManager{
	public:
	vector<GUI5constraint> constraints;
	GUI5constraintManager();
	
	//adds a new constraint after checking for no circular dependency
	void addConstraint(GUI5constraint C);	
	
	//returns true if, given the constraint, the element would depend on itself.
	bool isCircular(GUI5constraint C);	
	
	vector<GUI5constraint> ownedConstraints(GUI5base *element);
	vector<GUI5constraint> unownedConstraints(GUI5base *element);
	
	//these return the list of elements whose .area either depends on or is
	//depended on by the .area of this element.
	//Will split into "pos" and "size" later.
	set<GUI5base*> getsDirectly(GUI5base *element);
	set<GUI5base*> getsIndirectly(GUI5base *element);
	set<GUI5base*> setsDirectly(GUI5base *element);
	set<GUI5base*> setsIndirectly(GUI5base *element);
	
	//performs layout repair by activating the .invalidate() methods
	//of every constraint in the order of dependency.
	void invalidateElement(GUI5base *element); 
	
	//performs layout repair for the whole system.
	void invalidateAll();
};

#endif