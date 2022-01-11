#include "gui/Gui5/GUI5window.h"
#include "gui/Gui5/GUI5button.h"
#include "display/paint.h"
#include "resource/fonts.h"

void windowBtnCloseFunc(void *arg){
	((GUI5window*)arg)->close();
}

GUI5window::GUI5window(){
	colors["titlebar"] = (color4i){128,128,128,128};
	addElement((*(new GUI5button())).setText("X").setFunc(&windowBtnCloseFunc).setArg((void*)this).setSize({20,20}).setClient(false));
}

GUI5window &GUI5window::setTitle(string t){
	title = t;
	return *this;
}

void GUI5window::layout(){
	client_area.setStart({0,20});
	client_area.setEnd(area.getSize());
	GUI5base *btn = children[0];
	btn->area.moveEnd({area.getw(),20});
}

void GUI5window::render(){
	rect world = area.toWorld();
	//setColor({128,128,128});
	//setAlpha(64);
	setColorAlpha(colors["body"]);
	paintRect(world.getx(), world.gety()+20, world.getx2(), world.gety2());
	//setColor({0,0,0});
	//setAlpha(255);
	setColorAlpha(colors["border"]);
	paintRectOutline(world.getx(), world.gety()+20, world.getx2(), world.gety2());
	//setColor({128,128,128});
	//setAlpha(128);
	setColorAlpha(colors["titlebar"]);
	paintRect(world.getx(), world.gety(), world.getx2(), world.gety()+20);
	//setColor({0,0,0});
	//setAlpha(255);
	setColorAlpha(colors["border"]);
	paintRectOutline(world.getx(), world.gety(), world.getx2(), world.gety()+20);
	setColorAlpha(colors["text"]);
	printw(world.getx(), world.gety(), -1,-1,"%s",title.c_str());
	
	//GUI5base::render();
}

