#include "gui/Gui5/GUI5window.h"
#include "gui/Gui5/GUI5button.h"
#include "display/paint.h"
#include "resource/fonts.h"

void windowBtnCloseFunc(void *arg){
	((GUI5window*)arg)->close();
}

GUI5window::GUI5window(){
	addElement((*(new GUI5button())).setText(" X").setFunc((funcptr)&GUI5base::close).setArg((void*)this).setSize({20,20}).setClient(false));
	client_area.sety(20);
}

GUI5window &GUI5window::setTitle(string t){
	title = t;
	return *this;
}

void GUI5window::layout(){
	GUI5base *btn = children[0];
	btn->area.moveEnd({area.getw(),20});
	
	GUI5base::layout();
}

void GUI5window::render(){
	rect world = area.toWorld();
	setColor({128,128,128});
	setAlpha(64);
	paintRect(world.getx(), world.gety()+20, world.getx2(), world.gety2());
	setColor({0,0,0});
	setAlpha(255);
	paintRectOutline(world.getx(), world.gety()+20, world.getx2(), world.gety2());
	setColor({128,128,128});
	setAlpha(128);
	paintRect(world.getx(), world.gety(), world.getx2(), world.gety()+20);
	setColor({0,0,0});
	setAlpha(255);
	paintRectOutline(world.getx(), world.gety(), world.getx2(), world.gety()+20);
	printw(world.getx(), world.gety(), -1,-1,"%s",title.c_str());
	
	//GUI5base::render();
}

