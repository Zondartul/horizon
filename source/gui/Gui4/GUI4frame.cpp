#include "gui/GUI4/GUI4frame.h"
#include "display/paint.h"

void GUI4frame::render(){
	color3i color1 = {255*8/10,255*8/10,255};
	color3i color2 = {0,0,0};
	setAlpha(128);
	setColor(color1);
	paintRect(getVisibleWorldArea());//paintRect(area.toWorld());
	setColor(color2);
	paintRectOutline(getVisibleWorldArea());//paintRectOutline(area.toWorld());
	
	GUI4base::render();
	
}


	/* color3i color3 = {255,0,0};
	color3i color4 = {0,255,0};
	color3i color5 = {0,0,255};
	rect vis = getVisibleWorldArea();
	setColor(color4);
	paintRectOutline(area.start.x,area.start.y,area.end.x,area.end.y);
	setColor(color3);
	paintRectOutline(getVisibleWorldArea());
	setColor(color5);
	paintRectOutline(client_area.toWorld()); */