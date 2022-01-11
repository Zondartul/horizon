#include "gui/Gui5/GUI5label.h"
#include "resource/fonts.h"
#include "display/paint.h"

GUI5label &GUI5label::setText(string text){
	this->text = text;
	invalidate();
	return *this;
}

void GUI5label::render(){
	//printw(int x, int y, int xlim, int ylim, string format, ...);
	setColor({0,0,0});
	setAlpha(255);
	rect world = area.toWorld();
	printw(world.getx(), world.gety(),-1,-1,"%s",text.c_str());
	
	GUI5base::render();
}

void GUI5label::layout(){
	rect newsize;
	int chars;
	preprintw(&newsize, &chars, -1,-1,"%s",text.c_str());
	area.setSize(newsize.getSize());
	
	GUI5base::layout();
}