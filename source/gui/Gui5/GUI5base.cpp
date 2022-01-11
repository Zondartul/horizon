#include "gui/Gui5/GUI5base.h"
#include "display/paint.h"
#include "resource/fonts.h"

//class GUI5base:public messageReceiver
GUI5base::GUI5base(){
	parent = NULL;
	mouseover = false;
	debugmode = false;
	client_area.setParent(&area);
	client_draw_area.setParent(&area);
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
	A.area.setParent(&area);
	A.isClient = true;
	invalidate();
	return *this;
}
GUI5base &GUI5base::setDebug(bool debug){
	debugmode = debug;
	return *this;
}
GUI5base &GUI5base::setClient(bool client){
	isClient = client;
	return *this;
}
//signals
void GUI5base::think(){							//perform any logic not related to rendering
	for(int I = 0; I < children.size(); I++){
		children[I]->think();
	}
}
void GUI5base::renderlogic(){						//draw this element (and children)
	if(debugmode){debugrender();}
	else{render();}
	
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
	if(msg->type == "mouse_move"){
		//check mouseover
		vec2i newPos = msg->get<vec2i>(0);
		mouseover = area.toWorld().contains(newPos);
	}
	if((msg->type == "rmb_down") && mouseover){
		debugmode = !debugmode;
	}
	for(int I = 0; I < children.size(); I++){
		children[I]->receiveMessage(msg);
	}
}
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
	if(parent){
		vector<GUI5base*> &c = parent->children;
		for(int I = 0; I < c.size(); I++){
			if(c[I] == this){
				c.erase(c.begin()+I);
			}
		}
		parent->invalidate();
		//"delete this;" ?
	}
}