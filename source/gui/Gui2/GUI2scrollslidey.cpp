#include "gui/Gui2/GUI2scrollslidey.h"
#include "main/control.h"
#include "display/paint.h"
GUI2scrollslidey::GUI2scrollslidey():GUI2base()
{
	callback = NULL;
	arg = NULL;
	movable = true;
	vertical = true;
}
void GUI2scrollslidey::onClick(int mb)
{
	if(mb==1)
	{
		startTouch = mouseP-pos;
		if(callback)(callback(arg));
		printf("clicky.");
		if(movable){dragging = 1;}
	}
	else{dragging = 0;}
}
void GUI2scrollslidey::dragCheck()
{
	
	if(dragging)
	{
		vec2i newPos = mouseP-startTouch;
		if(vertical){newPos.x = pos.x;}else{newPos.y=pos.y;}
		
		if(parent)
		{
			newPos.x = clamp(newPos.x, parent->pos.x, parent->pos.x+parent->size.x-size.x);
			newPos.y = clamp(newPos.y, parent->pos.y, parent->pos.y+parent->size.y-size.y);
		}
		if(newPos!=pos)
		{
			invalidate(newPos, size);
			callback(arg);
		}	
	}
}
void GUI2scrollslidey::moveupdown(int dist)
{
	vec2i newPos = {pos.x+dist, pos.y+dist};
	if(vertical){newPos.x = pos.x;}else{newPos.y=pos.y;}
	
	if(parent)
	{
		newPos.x = clamp(newPos.x, parent->pos.x, parent->pos.x+parent->size.x-size.x);
		newPos.y = clamp(newPos.y, parent->pos.y, parent->pos.y+parent->size.y-size.y);
	}
	if(newPos!=pos)
	{
		invalidate(newPos, size);
		callback(arg);
	}	
}
void GUI2scrollslidey::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(mouseOver){setAlpha(255);}else{setAlpha(64);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	glDisable(GL_SCISSOR_TEST);
}