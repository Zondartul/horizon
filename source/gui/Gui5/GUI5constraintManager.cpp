#include "gui/Gui5/GUI5constraintManager.h"

GUI5constraintManager::GUI5constraintManager(){
}
	
//adds a new constraint after checking for no circular dependency
void GUI5constraintManager::addConstraint(GUI5constraint C){
	constraints.push_back(C);
}

//returns true if, given the constraint, the element would depend on itself.
bool GUI5constraintManager::isCircular(GUI5constraint C){
	return false;
}

vector<GUI5constraint> GUI5constraintManager::ownedConstraints(GUI5base *element){
	vector<GUI5constraint> Cs;
	for(int I = 0; I < constraints.size(); I++){
		if (constraints[I].self == element){
			Cs.push_back(constraints[I]);
		}
	}
}
vector<GUI5constraint> GUI5constraintManager::unownedConstraints(GUI5base *element){
	vector<GUI5constraint> V;
	return V;
}

//these return the list of elements whose .area either depends on or is
//depended on by the .area of this element.
//Will split into "pos" and "size" later.
set<GUI5base*> GUI5constraintManager::getsDirectly(GUI5base *element){
	set<GUI5base*> S;
	return S;
}
set<GUI5base*> GUI5constraintManager::getsIndirectly(GUI5base *element){
	set<GUI5base*> S;
	return S;
}
set<GUI5base*> GUI5constraintManager::setsDirectly(GUI5base *element){
	set<GUI5base*> S;
	return S;
}
set<GUI5base*> GUI5constraintManager::setsIndirectly(GUI5base *element){
	set<GUI5base*> S;
	return S;
}

//performs layout repair by activating the .invalidate() methods
//of every constraint in the order of dependency.
void GUI5constraintManager::invalidateElement(GUI5base *element){
	vector<GUI5constraint> Cs = ownedConstraints(element);
	for(int I = 0; I < Cs.size(); I++){
		Cs[I].invalidate();
	}
}

//performs layout repair for the whole system.
void GUI5constraintManager::invalidateAll(){}