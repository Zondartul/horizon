#include "gui/GUI4/GUI4base.h"
#include <algorithm>
#include "assert.h"
#include "display/paint.h"
//-#include "main/control.h"

//functions
GUI4base::GUI4base(){//constructor
	area.setStart({0,0}).setEnd({100,100});
	client_area.setStart({0,0}).setEnd({100,100});
	client_area.setParent(&area);
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
	is_client_area_constrained = false;
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
	vec2i S = area.getSize();
	area.setStart({x,y}).setSize(S);
	recalculateAnchor();
	invalidate();
	return *this;
}
GUI4base &GUI4base::setSize(int w, int h){
	//if(w < 0){w=0;}
	//if(h < 0){h=0;}
	//printf("setSize(%d,%d)\n",w,h);
	area.setSize({w,h});
	recalculateAnchor();
	invalidate();
	return *this;
}
GUI4base &GUI4base::setClientPos(int x, int y){
	vec2i S = client_area.getSize();
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

void GUI4base::recalculateAnchor(){
	switch(anchor_type_bottom){
		case(1)://move
			anchor_diff_bottom = area.getParent()->toWorld().gety2() - area.toWorld().gety();
		break;
		case(2)://scale
			anchor_diff_bottom = area.getParent()->toWorld().gety2() - area.toWorld().gety2();
		break;
	}
	switch(anchor_type_right){
		case(1)://move
			anchor_diff_right = area.getParent()->toWorld().getx2() - area.toWorld().getx();
		break;
		case(2)://scale
			anchor_diff_right = area.getParent()->toWorld().getx2() - area.toWorld().getx2();
		break;
	}
}

void GUI4base::checkAnchor(){
	if(parent){
		vec2i newPos = area.getStart();
		vec2i newSize = area.getSize();
		//check bottom anchor
		if(anchor_type_bottom != 0){
			int diffCurrent;
			int diffNeeded;
			switch(anchor_type_bottom){
			case(1)://move
				diffCurrent = area.getParent()->toWorld().gety2() - area.toWorld().gety();
				diffNeeded = anchor_diff_bottom;
				if(diffCurrent != diffNeeded){
					//printf("[%p]:anchor-bottom-move\n",this);
					newPos.y = newPos.y + diffCurrent - diffNeeded;
				}
			break;
			case(2)://scale
				diffCurrent = area.getParent()->toWorld().gety2() - area.toWorld().gety2();
				diffNeeded = anchor_diff_bottom;
				if(diffCurrent != diffNeeded){
					//printf("[%p]:anchor-bottom-scale\n",this);
					newSize.y = newSize.y + diffCurrent - diffNeeded;
				}
			break;
			}
		}
		//check right anchor
		if(anchor_type_right != 0){
			int diffCurrent;
			int diffNeeded;
			switch(anchor_type_right){
			case(1)://move
				diffCurrent = area.getParent()->toWorld().getx2() - area.toWorld().getx();
				diffNeeded = anchor_diff_right;
				if(diffCurrent != diffNeeded){
					//printf("[%p]:anchor-right-move\n",this);
					newPos.x = newPos.x + diffCurrent - diffNeeded;
				}
			break;
			case(2)://scale
				diffCurrent = area.getParent()->toWorld().getx2() - area.toWorld().getx2();
				diffNeeded = anchor_diff_right;
				if(diffCurrent != diffNeeded){
					//printf("[%p]:anchor-right-scale\n",this);
					newSize.x = newSize.x + diffCurrent - diffNeeded;
				}
			break;
			}
		}
		setPos(newPos.x,newPos.y);
		setSize(newSize.x,newSize.y);
	}
}

GUI4base &GUI4base::setAnchor(int bottom_anchor, int right_anchor){
	//options:
	//0: ignore-bottom
	//1: move-with-bottom
	//2: scale-with-bottom
	//---------
	//0: ignore-right
	//1: move-with-right
	//2: scale-with-right
	anchor_type_bottom = bottom_anchor;
	anchor_type_right = right_anchor;
	if(parent){recalculateAnchor();}
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
GUI4base &GUI4base::setClientAreaConstrained(bool value){
	is_client_area_constrained = value;
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
		invalidate_suppressed = true;
		//start with fixing the client area
		/* if(is_client_area_constrained){
			for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
				if((*I)->is_client){
					rect newPos = client_area.insert((*I)->area);
					(*I)->setPos(newPos.start.x,newPos.start.y);
				}
			}
		} */
		checkAnchor();
		//	first, see if we have an order to keep
		int x = 0;
		int y = 0;
		switch(client_order){
			case ORDER_NONE:
				break;
			case ORDER_HORIZONTAL:
					for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
						(*I)->setPos(x,0);
						x = (*I)->area.getx2()+1;
					}
				break;
			case ORDER_VERTICAL:
					for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
						(*I)->setPos(0,y);
						y = (*I)->area.gety2()+1;
					}
				break;
		}
		//	next, see if we need to resize based on client size.
		if(size_to_contents){checkSizeToContents();}
		
		//	next, see if we're attached to anything.
		// if...
		//finally, invalidate all the children.
		for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
			(*I)->invalidate();
		}
		invalidate_suppressed = false;
	}
}
//add a child
GUI4base &GUI4base::addElement(GUI4base *A){
	children.push_back(A);
	A->parent = this;
	if(A->is_client){
		A->area.setParent(&client_area);
	}else{
		A->area.setParent(&area);
	}
	A->recalculateAnchor();
}

void GUI4base::checkSizeToContents(){
	//printf("[%p]:G4base::cSTC\n",this);
	int maxx = 0;
	int maxy = 0;
	int x;
	int y;
	for(vector<GUI4base*>::iterator I = children.begin(); I != children.end(); I++){
		if((*I)->is_client){
			x = (*I)->area.getx2();
			y = (*I)->area.gety2();
			if(x > maxx){maxx = x;}
			if(y > maxy){maxy = y;}
		}
	}
	if((maxx != client_area.getx2())||(maxy != client_area.gety2())){
		//setSize(maxx+area.end.x-client_area.end.x,maxy+area.end.y-client_area.end.y);
		vec2i diff = area.getEnd()-client_area.getEnd();
		vec2i end = {maxx,maxy};
		client_area.setEnd(end);
		area.setEnd(end+diff);
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
				rect vis = area.toWorld();//getVisibleWorldArea();
				vec2i mouse = input.getMousePos();
				//record initial mouse and window positions
				click_pos = mouse;						
				prev_area = area.toWorld();
				client_diff = area.getSize()-client_area.getSize();
				if(vis.contains(mouse)){
					//was the click on one of the edges?
					resize_edge = 0;
					if(!vis.contains(mouse+(vec2i){2,0})){click_state = 3;resize_edge += 1;}
					if(!vis.contains(mouse+(vec2i){0,2})){click_state = 3;resize_edge += 2;}
					if(!vis.contains(mouse+(vec2i){-2,0})){click_state = 3;resize_edge += 4;}
					if(!vis.contains(mouse+(vec2i){0,-2})){click_state = 3;resize_edge += 8;}
					if(resize_edge == 0){
						//not on the edge.
						click_state = 1;
					}
				}else{
					//else ignore it until it un-clicks
					click_state = 2;
				}
			}
		break;
		//if we are dragged, resized, or clicked:
		case 1:
		case 3:
			//if mouse is still being held
			if(input.mouse1down){
				//get current mouse position
				vec2i mouse = input.getMousePos();
				/*
				if(parent){ //confine the mouse
					if(is_client){mouse = parent->client_area.toWorld().clamp(mouse);}
					else{	      mouse = parent->area.toWorld().clamp(mouse);       }
				}
				*/
				
				//if the mouse moved at all
				if(mouse != click_pos){
					//figure out how the mouse should be constrained
					//to prevent weird positions
					rect screen = getScreenRect();
					vec2i c_start = screen.getStart();
					vec2i c_end = screen.getEnd();
					if(parent){
						c_start = area.getParent()->toWorld().getStart();
						c_end = area.getParent()->toWorld().getEnd();
						/* if(is_client){
							c_start = parent->client_area.toWorld().start;
							c_end = parent->client_area.toWorld().end;
						}else{
							c_start = parent->area.toWorld().start;
							c_end = parent->area.toWorld().end;
						} */
						//start is pretty much always {0,0}
						//relative to whatever the parent is
						//BUT we are working in world coords
					}
					if(is_movable && click_state == 1){
						//drag condition
						c_start = c_start+(click_pos-prev_area.getStart());
						c_end = c_end+(click_pos-prev_area.getEnd());
						/* if(parent){
							if(is_client){c_end = parent->client_area.toWorld().end+(mouse-area.toWorld().end);}
							else{c_end = parent->area.toWorld().end+(mouse-area.toWorld().end);}
						} */
						
					}else if(is_resizible && click_state == 3){
						if(resize_edge & 1){ //right edge
							//start is left edge
							//end is parent area end
							c_start.x = prev_area.getx();
						}else if(resize_edge & 4){ //left edge
							//start is parent area start
							//end is right edge
							c_end.x = prev_area.getx2();
						}
						if(resize_edge & 2){ //bottom edge
							//start is top edge
							//end is parent area end
							c_start.y = prev_area.gety();
						}else if(resize_edge & 8){ //top edge
							//start is parent area start
							//end is bottom edge
							c_end.y = prev_area.gety2();
						}
						
					}
					mouse = clamp(mouse,c_start,c_end);
					paintCircle(mouse.x,mouse.y,2);
					//should we get dragged?
					if(is_movable && click_state == 1){
						
						//calculate the change in mouse position
						//and fromt that the new window position
						vec2i newV = (prev_area.getStart()+mouse-click_pos);
						//convert to propper coordinate system
						if(parent){newV = parent->area.fromWorld(newV);}
						//move window
						setPos(newV.x, newV.y);
					}
					//should we get resized?
					if(is_resizible && click_state == 3){
						//calculate the change in mouse position
						vec2i diff = mouse-click_pos;
						//calculate the change in size
						vec2i newSize = prev_area.getSize();
						//for left and top edge, we combine move and resize.
						vec2i newPos = prev_area.getStart();
						if(resize_edge & 1){newSize.x = prev_area.getw()+diff.x;}
						else if(resize_edge & 4){newSize.x = prev_area.getw()-diff.x; newPos.x = prev_area.getx()+diff.x;}
						if(resize_edge & 2){newSize.y = prev_area.geth()+diff.y;}
						else if(resize_edge & 8){newSize.y = prev_area.geth()-diff.y; newPos.y = prev_area.gety()+diff.y;}
						//convert to propper coordinate system
						if(parent){newPos = parent->area.fromWorld(newPos);}
						//move window
						//setPos(newPos.x, newPos.y);
						//resize window
						setSize(newSize.x, newSize.y);
						//setClientSize(newSize.x-client_diff.x, newSize.y-client_diff.y);
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
























