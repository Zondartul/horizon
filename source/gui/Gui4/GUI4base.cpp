#include "gui/GUI4/GUI4base.h"
#include <algorithm>
#include "assert.h"

//functions
GUI4base::GUI4base(){//constructor
	area.setStart({0,0}).setEnd({100,100});
	client_area.setStart({0,0}).setEnd({100,100});
	client_area.parent = &area;
	parent = NULL;
	children.clear();
	client_order = ORDER_NONE;
	size_to_contents = false;
	is_visible = true;
	is_clickable = false;
	is_resizible = false;
	is_movable = false;
	invalidate_suppressed = false;
	is_client = true;
	click_state = 0;
}
GUI4base::~GUI4base(){} //destructor
/*
vec2i GUI4base::ParentToWindow(vec2i A){
	int X = A.x-area.x;
	int Y = A.y-area.y;
	if(parent && is_client){return parent->WindowToClient({X,Y});}
	else{return {X,Y};}
}
vec2i GUI4base::WorldToWindow(vec2i A){
	if(parent){return ParentToWindow(parent->WorldToWindow(A));}
	else{return ParentToWindow(A);}
}
vec2i GUI4base::WindowToClient(vec2i A){
	int X = A.x-client_area.x;
	int Y = A.y-client_area.y;
	return {X,Y};
}
vec2i GUI4base::ClientToWindow(vec2i A){
	int X = A.x+client_area.x;
	int Y = A.y+client_area.y;
	return {X,Y};
}
vec2i GUI4base::WindowToWorld(vec2i A){
	if(parent){return parent->WindowToWorld(WindowToParent(A));}
	else{return WindowToParent(A);}
}
vec2i GUI4base::WindowToParent(vec2i A){
	int X = A.x+area.x;
	int Y = A.y+area.y;
	if(parent && is_client){return parent->ClientToWindow({X,Y});}
	else{return {X,Y};}
}
rect GUI4base::getVisibleWorldArea(){
	vec2i start = WindowToWorld({0,0});
	vec2i start2 = start;
	vec2i end = WindowToWorld({area.w,area.h});
	vec2i end2 = end;
	if(parent && is_client){
		start2 = parent->WindowToWorld({parent->client_area.x,parent->client_area.y});
		end2 = parent->WindowToWorld({parent->client_area.x2,parent->client_area.y2});
	}
	rect result;
	return result.setx(max(start.x,start2.x)).sety(max(start.y,start2.y)).setx2(min(end.x,end2.x)).sety2(min(end.y,end2.y));
}
*/
rect GUI4base::getVisibleWorldArea(){
	if(parent){
		if(is_client){
			return parent->client_area.toWorld().clamp(area.toWorld());//parent->client_area.clamp(area.toParent()).toWorld();
		}else{
			return parent->area.toWorld().clamp(area.toWorld());//parent->area.clamp(area.toParent()).toWorld();
		}
	}else{
		return area;
	}
}
//mutators
GUI4base &GUI4base::setPos(int x, int y){
	//area.setx(x).sety(y);
	vec2i S = area.size;
	area.setStart({x,y}).setSize(S);
	invalidate();
	return *this;
}
GUI4base &GUI4base::setSize(int w, int h){
	area.setSize({w,h});
	invalidate();
	return *this;
}
GUI4base &GUI4base::setClientPos(int x, int y){
	vec2i S = client_area.size;
	client_area.setStart({x,y}).setSize(S);
	invalidate();
	return *this;
}
GUI4base &GUI4base::setClientSize(int w, int h){
	client_area.setSize({w,h});
	invalidate();
	return *this;
}
GUI4base &GUI4base::setClientOrder(GUI4orderType order){
	client_order = order;
	invalidate();
	return *this;
}
GUI4base &GUI4base::setSizeToContents(bool value){
	size_to_contents = value;
	invalidate();
	return *this;
}
/*
GUI4base &GUI4base::sizeToContents(bool value){
	
}
*/
GUI4base &GUI4base::sortContents(bool (*compare)(GUI4base *A, GUI4base *B)){
	stable_sort(children.begin(), children.end(), compare);
	return *this;
}

GUI4base &GUI4base::setAnchor(GUI4direction point1, GUI4direction point2){
	anchor_point1 = point1;
	anchor_point2 = point2;
	return *this;
}
GUI4base &GUI4base::setVisible(bool value){
	is_visible = value;
	return *this;
}
GUI4base &GUI4base::setClickable(bool value){
	is_clickable = value;
	return *this;
}
GUI4base &GUI4base::setResizible(bool value){
	is_resizible = value;
	return *this;
}
GUI4base &GUI4base::setMovable(bool value){
	is_movable = value;
	return *this;
}
GUI4base &GUI4base::setClient(bool value){
	if(parent){assert(!"GUI4: do not change element's client status after it's been added");}
	is_client = value;
	return *this;
}
//signals
//delete the object
void GUI4base::close(){
	if(parent){
		for(vector<GUI4base*>::iterator I = parent->children.begin(); I != parent->children.end(); I++){
			if(*I == this){parent->children.erase(I);break;}
		}
	}
} 
//apply changes to external positioning
void GUI4base::invalidate(){
	//first, figure out own positioning data
	invalidateSelf();
	//then, tell parent to fix the ordering and other such things
	if(parent){parent->invalidateSelf();}
} 
//apply changes to own positioning
void GUI4base::invalidateSelf(){
	//first off, make sure we are not called recursively.
	//this is how we differentiate between changes called by user and those called
	//by other gui elements.
	if(!invalidate_suppressed){
		printf("invalidating");
		invalidate_suppressed = true;
		//start with fixing the client area
		//	first, see if we have an order to keep
		int x = 0;
		int y = 0;
		switch(client_order){
			case ORDER_NONE:
				break;
			case ORDER_HORIZONTAL:
					for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
						(*I)->setPos(x,0);
						x = (*I)->area.end.x+1;
					}
				break;
			case ORDER_VERTICAL:
					for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
						(*I)->setPos(0,y);
						y = (*I)->area.end.y+1;
					}
				break;
		}
		//	next, see if we need to resize based on client size.
		if(size_to_contents){
			int maxx = 0;
			int maxy = 0;
			int x;
			int y;
			for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
				if((*I)->is_client){
					x = (*I)->area.end.x;
					y = (*I)->area.end.y;
					if(x > maxx){maxx = x;}
					if(y > maxy){maxy = y;}
				}
			}
			if((maxx != client_area.end.x)||(maxy != client_area.end.y)){
				//setSize(maxx+area.end.x-client_area.end.x,maxy+area.end.y-client_area.end.y);
				vec2i diff = area.end-client_area.end;
				vec2i end = {maxx,maxy};
				client_area.setEnd(end);
				area.setEnd(end+diff);
			}
		}
		//	next, see if we're attached to anything.
		// if...
		invalidate_suppressed = false;
	}
}
//add a child
GUI4base &GUI4base::addElement(GUI4base *A){
	children.push_back(A);
	A->parent = this;
	if(A->is_client){
		A->area.parent = &client_area;
	}else{
		A->area.parent = &area;
	}
}
//draw the element
void GUI4base::render(){
	for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
		(*I)->render();
	}
}
void GUI4base::think(){
	switch(click_state){
		case 0:
			//if mouse 1 was clicked
			if(input.mouse1down){
				//was the click inside window?
				if(getVisibleWorldArea().contains(input.getMousePos())){
				click_state = 1;
				//record initial mouse and window positions
				click_pos = input.getMousePos();						
				prev_win_pos = area.toWorld().start;
				}else{
					//else ignore it until it un-clicks
					click_state = 2;
				}
			}
		break;
		case 1:
			//if mouse is still being held
			if(input.mouse1down){
				//are we allowed to be dragged by mouse?
				if(is_movable){
					//get current mouse position
					vec2i V = input.getMousePos();
					if(parent){
						//do not allow the mouse to drag us outside our bounds
						if(is_client){V = parent->client_area.toWorld().clamp(V);}
						else{	      V = parent->area.toWorld().clamp(V);       }
					}
					//if the mouse moved at all
					if(V != click_pos){
						//calculate the change in mouse position
						//and fromt that the new window position
						vec2i newV = (prev_win_pos+V-click_pos);
						//convert to propper coordinate system
						if(parent){newV = parent->area.fromWorld(newV);}
						//move window
						setPos(newV.x, newV.y);
					}
				}
			//if mouse is released
			}else{
				click_state = 0;
				//are we allowed to be clicked?
				if(is_clickable && onMouseClick){(*onMouseClick)();}
			}
		break;
		case 2:
			//wait for mouse1 to be released
			if(!input.mouse1down){
				click_state = 0;
			}
		break;
	}
	//let all the children run their logic too
	for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
		(*I)->think();
	}
}
//events
//GUI4base &GUI4base::onMouseover(){}
//GUI4base &GUI4base::onMouseClick(){}
//GUI4base &GUI4base::onKeyboard(){}
























