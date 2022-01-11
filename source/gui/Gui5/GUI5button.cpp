#include "gui/Gui5/GUI5button.h"
#include "resource/fonts.h"
#include "display/paint.h"

GUI5button::GUI5button(){
	pressed = 0;
	func = 0;
	arg = 0;
}

GUI5button &GUI5button::setText(string text){
	this->text = text;
	invalidate();
	return *this;
}

GUI5button &GUI5button::setFunc(funcptr f){
	func = f;
	return *this;
}
GUI5button &GUI5button::setArg(void *a){
	arg = a;
	return *this;
}

void GUI5button::receiveMessage(message *msg){
	GUI5base::receiveMessage(msg);
	if(msg->type == "lmb_down" && mouseover){pressed = true;}
	if(msg->type == "lmb_up" && pressed){pressed = false; if(func){func(arg);}}
}

void GUI5button::render(){
	//printw(int x, int y, int xlim, int ylim, string format, ...);
	
	if(pressed){
		setColor({200,200,200});
	}else{
		setColor({128,128,128});
	}
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
	
	setColor({0,0,0});
	setAlpha(255);
	printw(world.getx(), world.gety(),-1,-1,"%s",text.c_str());
	
	//GUI5base::render();
}

void GUI5button::layout(){
	if(area == rect()){
		rect newsize;
		int chars;
		preprintw(&newsize, &chars, -1,-1,"%s",text.c_str());
		area.setSize(newsize.getSize());
	}
	//GUI5base::layout();
}