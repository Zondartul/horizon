#include "gui/Gui5/GUI5base.h"
#include "display/paint.h"
#include "resource/fonts.h"
#include "input/inputMessage.h"

//class GUI5base:public messageReceiver
GUI5base::GUI5base(){
	parent = NULL;
	mouseover = false;
	debugmode = false;
	hidden = false;
	channel.stopWhenHandled = false;
	isClient = true;
	deletePending = false;
	movable = true;
	moving = false;
	client_area.setParent(&area);
}
//mutators
GUI5base &GUI5base::setPos(vec2i pos){			//change position (with side-effects)
	area.moveStart(pos);
	invalidate();
	return *this;
}
GUI5base &GUI5base::setSize(vec2i size){		//change size	(with side-effects)
	area.setSize(size);
	invalidate();
	return *this;
}
GUI5base &GUI5base::addElement(GUI5base &A){	//add a child element
	children.push_back(&A);
	A.parent = this;
	if(A.isClient){
		A.area.setParent(&client_area);
	}else{
		A.area.setParent(&area);
	}
	//A.isClient = true;
	A.subscribeToMessageChannel(&channel,"");	//and pay taxes!
	invalidate();
	return *this;
}
GUI5base &GUI5base::removeElement(GUI5base &A){
	for(int I = 0; I < children.size(); I++){
		if(children[I] == &A){
			children.erase(children.begin()+I);
			A.unsubscribeFromMessageChannel(&channel,"");
		}
	}
	return *this;
}
GUI5base &GUI5base::removeAndDeleteElement(GUI5base &A){
	removeElement(A);
	delete &A;
}
GUI5base &GUI5base::setDebug(bool debug){
	debugmode = debug;
	return *this;
}
GUI5base &GUI5base::setClient(bool client){
	isClient = client;
	return *this;
}
GUI5base &GUI5base::setHidden(bool hidden){
	this->hidden = hidden;
	return *this;
}
GUI5base &GUI5base::setMovable(bool movable){
	this->movable = movable;
	return *this;
}
//getters
rect GUI5base::getVisibleArea(){ //returns actual drawn area, not just the scissor area
	if(parent){
		return parent->getVisibleArea().clamp(area.getParent()->toWorld().clamp(area.toWorld()));
	}else{
		return area;
	}
}
//signals
void GUI5base::think(){							//perform any logic not related to rendering
	for(int I = 0; I < children.size(); I++){
		children[I]->think();
		//honestly not sure where else to put this
		if(children[I]->deletePending){
			removeAndDeleteElement(*children[I]);
			invalidate();
		}
	}
}
void GUI5base::renderlogic(){						//draw this element (and children)
	if(hidden){return;}
	tempPaintSettings t;	//hi constructor side-effects
	t.setScissorEnabled(true);
	if(debugmode){debugrender();}
	else{
		if(parent){
			t.crop(area.getParent()->toWorld());
		}
		render();	
	}
	
	for(int I = 0; I < children.size(); I++){
		children[I]->renderlogic();//so inheriting elements don't have to worry about it
	}
}
void GUI5base::render(){}						//I'm invisible, you can't see me

void GUI5base::debugrender(){					//this will be used instead in debugmode
	setColor({128,128,128});
	setAlpha(64);
	rect world = area.toWorld();
	paintRect(world.getx(), world.gety(), world.getx2(), world.gety2());
	if(mouseover){
		setColor({255,255,255});
	}else{
		setColor({0,0,0});
	}
	setAlpha(255);
	paintRectOutline(world.getx(), world.gety(), world.getx2(), world.gety2());
	printw(world.getx(), world.gety(), -1,-1,"size: %dx%d\n(%d,%d)-(%d,%d)",world.getw(),world.geth(),world.getx(), world.gety(), world.getx2(), world.gety2());
}
void GUI5base::receiveMessage(message *msg){	//react to message, e.g. mouse click
	if(hidden){return;}
	message *newmsg = msg->copy();
	newmsg->handled = false;
	channel.publish(newmsg);
	if(msg->type == "mouse_move"){
		if(moving){
			vec2i deltaPos = ((message_mouse_move*)msg)->deltaPos;
			vec2i newPos = ((message_mouse_move*)msg)->newPos;
			if(parent){newPos = area.getParent()->toWorld().clamp(newPos);}
			area.moveStart(moveOffset + newPos);
			invalidate();
		}
		//check mouseover
		vec2i newPos = ((message_mouse_move*)msg)->newPos;
		if(getVisibleArea().contains(newPos)){
			mouseover = !newmsg->handled; //if noone has mouseover, i do
			msg->handled = true;
		}else{
			mouseover = false;
		}
	}
	if((msg->type == "rmb_down") && mouseover){
		debugmode = !debugmode;
	}
	if((msg->type == "lmb_down") && mouseover && movable){
		moving = true;
		moveOffset = area.getStart() - ((message_mouse*)msg)->newPos;
	}
	if(msg->type == "lmb_up"){
		moving = false;
	}
	delete newmsg;
	receiveMessageExtra(msg);
}
void GUI5base::receiveMessageExtra(message *msg){}
void GUI5base::invalidate(){						//inform the element that layout-altering change has occurred
	if(parent){parent->invalidate();}				//pretend that I know what i'm doing
	else{layoutlogic();}							//first we reach the top of the tree, then layout downwards
}													//this is super wrong but I'll figure out something later
void GUI5base::layoutlogic(){						//perform layout logic
	layout();
	for(int I = 0; I < children.size(); I++){
		children[I]->layoutlogic();
	}
}
void GUI5base::layout(){
	client_area.setStart({0,0});
	client_area.setSize(area.getSize());
}
void GUI5base::close(){
	deletePending = true;
	/* if(parent){
		unsubscribeFromMessageChannel(&parent->channel, "");
		vector<GUI5base*> &c = parent->children;
		for(int I = 0; I < c.size(); I++){
			if(c[I] == this){
				c.erase(c.begin()+I);
			}
		}
		parent->invalidate();
		delete this;
	} */
}