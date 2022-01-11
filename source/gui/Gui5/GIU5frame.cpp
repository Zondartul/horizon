#include "gui/Gui5/GUI5frame.h"
#include "display/paint.h"

GUI5frame::GUI5frame(){
	area.setStart({0,0});
	area.setSize({100,100});
}

void GUI5frame::render(){
	// setColor({128,128,128});
	// setAlpha(64);
	setColorAlpha(colors["body"]);
	rect world = area.toWorld();
	paintRect(world);
	// setColor({0,0,0});
	// setAlpha(255);
	setColorAlpha(colors["border"]);
	paintRectOutline(world);
}