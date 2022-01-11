#include "gui/Gui5/GUI5base.h"
#include "display/paint.h"
#include "resource/fonts.h"
#include "input/inputMessage.h"
#include "resource/cursor.h"
#include "gui/Gui5/behaviors/GUI5behaviorMovable.h"
#include "gui/Gui5/behaviors/GUI5behaviorScalable.h"
#include "gui/Gui5/behaviors/GUI5behaviorChildren.h"
#include "gui/Gui5/behaviors/GUI5behaviorDebug.h"
#include "gui/Gui5/behaviors/GUI5behaviorHighlight.h"

//class GUI5base:public messageReceiver
GUI5base::GUI5base(){
	parent = NULL;
	debugmode = false;
	hidden = false;
	channel.stopWhenHandled = false;
	isClient = true;
	deletePending = false;
	fill = false;
	client_area.setParent(&area);
	colors["border"] = (color4i){0,0,0,255};
	colors["body"] = (color4i){128,128,128,64};
	colors["text"] = (color4i){0,0,0,255};
	behaviors.push_back(new GUI5behaviorScalable());
	behaviors.push_back(new GUI5behaviorChildren());
	behaviors.push_back(new GUI5behaviorMovable());
	behaviors.push_back(new GUI5behaviorDebug());
	behaviors.push_back(new GUI5behaviorHighlight());
}
GUI5base::~GUI5base(){
	printf("destroying %s, son of %s\n",toString(this).c_str(),toString(parent).c_str());
	for(int I = 0; I < children.size(); I++){
		delete children[I];
	}
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
GUI5base &GUI5base::setPercentageSize(float mulx, float muly, int addx, int addy){
	vec2i parentSize;
	if(parent){
		parentSize = area.getParent()->getSize();
	}else{
		parentSize = getScreenRect().getSize();
	}
	setSize({(int)(mulx*parentSize.x/100.0f+addx), (int)(muly*parentSize.y/100.0f+addy)});
	return *this;
}
GUI5base &GUI5base::setPercentageSize(float mulx, float muly){
	setPercentageSize(mulx, muly, 0, 0);
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
	printf("closing %s\n",toString(&A).c_str());
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
GUI5base &GUI5base::setFill(bool fill){
	this->fill = fill;
	return *this;
}
//getters
rect GUI5base::getVisibleArea(){ //returns actual drawn area, not just the scissor area
	if(parent){
		if(isClient){
			return parent->getVisibleArea().clamp(parent->client_area.toWorld()).clamp(area.toWorld());
		}else{
			return parent->getVisibleArea().clamp(area.toWorld());
		}
	}else{
		return area.toWorld();
	}
}
bool GUI5base::hasAnyMouseover(vec2i mousepos){			//the specified position is within the visible area
	return !hidden && getVisibleArea().contains(mousepos);
}
bool GUI5base::hasExclusiveMouseover(vec2i mousepos){		//the specified position is within the visible area 
	bool nonexclusive = false;									//  but not within that of any child elements
	if(hasAnyMouseover(mousepos)){
		for(int I = 0; I < children.size(); I++){
			if(children[I]->hasAnyMouseover(mousepos)){
				return false;
			}
		}
		return true;
	}else{
		return false;
	}
}
												//signals
void GUI5base::think(){							//perform any logic not related to rendering
	for(int I = 0; I < children.size(); I++){
		children[I]->think();
		//honestly not sure where else to put this
		if(children[I]->deletePending){
			removeAndDeleteElement(*children[I]);
			I--;
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
		//if(parent){
			//t.crop(area.getParent()->toWorld());
		//}
		render();	
	}
	//non-clients render anywhere, clients render only in client area
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient){
			t.setScissor(children[I]->getVisibleArea());
		}else{
			t.setScissor(getVisibleArea());
		}
		children[I]->renderlogic();//so inheriting elements don't have to worry about it
	}
}
void GUI5base::render(){}						//I'm invisible, you can't see me

void GUI5base::debugrender(){					//this will be used instead in debugmode
	//setColor({128,128,128});
	//setAlpha(64);
	setColorAlpha(colors["body"]);
	rect world = area.toWorld();
	paintRect(world.getx(), world.gety(), world.getx2(), world.gety2());
	/*
	if(mouseover){
		setColor({255,255,255});
	}else{
		setColor({0,0,0});
	}*/
	//setColor({0,0,0});
	//setAlpha(255);
	setColorAlpha(colors["border"]);
	paintRectOutline(world.getx(), world.gety(), world.getx2(), world.gety2());
	setColorAlpha(colors["text"]);
	printw(world.getx(), world.gety(), -1,-1,"size: %dx%d\n(%d,%d)-(%d,%d)",world.getw(),world.geth(),world.getx(), world.gety(), world.getx2(), world.gety2());
}
void GUI5base::receiveMessage(message *msg){	//react to message, e.g. mouse click
	if(hidden){return;}
	for(int I = 0; (I < behaviors.size()) && (!msg->handled); I++){behaviors[I]->run(this, msg);}
	//if(!parent){setCursor(getCursorHandle("arrow"));}
	//message *newmsg = msg->copy();
	//newmsg->handled = false;
	//channel.publish(newmsg);

	//delete newmsg;
	receiveMessageExtra(msg);
}
void GUI5base::receiveMessageExtra(message *msg){}
void GUI5base::invalidate(){						//inform the element that layout-altering change has occurred
	if(parent){parent->invalidate();}				//pretend that I know what i'm doing
	else{layoutlogic();}							//first we reach the top of the tree, then layout downwards
}													//this is super wrong but I'll figure out something later
void GUI5base::layoutlogic(){						//perform layout logic
	if(fill){
			vec2i parentSize;
		if(parent){
			parentSize = area.getParent()->getSize();
		}else{
			parentSize = getScreenRect().getSize();
		}
		area.setStart({0,0}).setSize(parentSize);
	}
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
}