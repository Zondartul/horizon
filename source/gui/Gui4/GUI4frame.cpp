#include "gui/GUI4/GUI4frame.h"
#include "display/paint.h"

void GUI4frame::render(){
	color3i color1 = {255*8/10,255*8/10,255};
	color3i color2 = {0,0,0};
	vec2i start = WindowToWorld({0,0});
	vec2i end = WindowToWorld({area.w,area.h});
	setColor(color1);
	paintRect(start.x,start.y,end.x,end.y);
	setColor(color2);
	paintRectOutline(start.x,start.y,end.x,end.y);
	color3i color3 = {255,0,0};
	color3i color4 = {0,255,0};
	color3i color5 = {0,0,255};
	rect vis = getVisibleWorldArea();
	setColor(color4);
	paintRectOutline(area.x,area.y,area.x2,area.y2);
	setColor(color3);
	paintRectOutline(vis.x,vis.y,vis.x2,vis.y2);
	setColor(color5);
	vec2i start2 = WindowToWorld({client_area.x,client_area.y});
	vec2i end2 = WindowToWorld({client_area.x2,client_area.y2});
	paintRectOutline(start2.x,start2.y,end2.x,end2.y);
	GUI4base::render();
}

