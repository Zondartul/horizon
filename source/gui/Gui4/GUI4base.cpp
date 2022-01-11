#include "gui/GUI4/GUI4base.h"
#include <algorithm>

//functions
GUI4base::GUI4base(){} //constructor
GUI4base::~GUI4base(){} //destructor
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
//mutators
GUI4base &GUI4base::setPos(int x, int y){
	area.setx(x).sety(y);
	return *this;
}
GUI4base &GUI4base::setSize(int w, int h){
	area.setw(w).seth(h);
	return *this;
}
GUI4base &GUI4base::setClientPos(int x, int y){
	client_area.setx(x).sety(y);
	return *this;
}
GUI4base &GUI4base::setClientSize(int w, int h){
	client_area.setw(w).seth(h);
	return *this;
}
GUI4base &GUI4base::setClientOrder(GUI4orderType order){
	client_order = order;
	return *this;
}
GUI4base &GUI4base::setSizeToContents(bool value){
	size_to_contents = value;
}
/*
GUI4base &GUI4base::sizeToContents(bool value){
	
}
*/
GUI4base &GUI4base::sortContents(bool (*compare)(GUI4base *A, GUI4base *B)){
	stable_sort(children.begin(), children.end(), compare);
}

GUI4base &GUI4base::setAnchor(GUI4direction point1, GUI4direction point2){
	anchor_point1 = point1;
	anchor_point2 = point2;
}
GUI4base &GUI4base::setVisible(bool value){
	is_visible = value;
}
GUI4base &GUI4base::setClickable(bool value){
	is_clickable = value;
}
GUI4base &GUI4base::setResizible(bool value){
	is_resizible = value;
}
GUI4base &GUI4base::setMovable(bool value){
	is_movable = value;
}
GUI4base &GUI4base::setClient(bool value){
	is_client = value;
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
						x = (*I)->area.x2+1;
					}
				break;
			case ORDER_VERTICAL:
					for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
						(*I)->setPos(0,y);
						y = (*I)->area.y2+1;
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
					x = (*I)->area.x2;
					y = (*I)->area.y2;
					if(x > maxx){maxx = x;}
					if(y > maxy){maxy = y;}
				}
			}
			if((maxx != client_area.x2)||(maxy != client_area.y2)){
				setSize(maxx+area.x2-client_area.x2,maxy+area.y2-client_area.y2);
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
}
//draw the element
void GUI4base::render(){
	for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
		(*I)->render();
	}
}
void GUI4base::think(){
	if(click_state == 0){
		if(input.mouse1down && getVisibleWorldArea().contains(input.getMousePos())){
																	// area = parent relative
			click_state = 1;										// mousepos = world relative
			click_pos = input.getMousePos();						// world -> window -> parent
			prev_win_pos = WindowToWorld({0,0});
		}
	}else{
		if(input.mouse1down){
			if(is_movable){
				vec2i V = input.getMousePos();
				if(parent){V = WindowToWorld(parent->client_area.clamp(WorldToWindow(V)));}
				if(V != click_pos){
					vec2i newV = (prev_win_pos+V-click_pos);
					if(parent){newV = parent->WorldToWindow(newV);}
					setPos(newV.x, newV.y);
				}
			}
		}else{
			click_state = 0;
			if(is_clickable && onMouseClick){(*onMouseClick)();}
		}
	}
	for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
		(*I)->think();
	}
}
//events
//GUI4base &GUI4base::onMouseover(){}
//GUI4base &GUI4base::onMouseClick(){}
//GUI4base &GUI4base::onKeyboard(){}
























