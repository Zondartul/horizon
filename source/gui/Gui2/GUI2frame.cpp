#include "gui/Gui2/GUI2frame.h"
#include "display/paint.h"
#include "resource/fonts.h"
void GUI2frame::btnClose(void* arg)
{
	delete ((GUI2frame*)arg);
}
GUI2frame::GUI2frame():GUI2base()
{
	title = "Title";
	CloseButton = new GUI2button();
	CloseButton->func = &btnClose;
	CloseButton->movable = false;
	CloseButton->resizible = false;
	CloseButton->scissor = false; //see we can't paint it outside client area and that's a problem.
	CloseButton->setSize(24,24);
	CloseButton->arg = (void*)this;
	CloseButton->setParent((GUI2base*)this);
	CloseButton->tag = "btnClose";
}
void GUI2frame::invalidate(vec2i newPos, vec2i newSize)
{
	GUI2base::invalidate(newPos, newSize);
	CloseButton->setPos(pos.x+size.x-border-CloseButton->size.x,pos.y+border);
}


void GUI2frame::recalculateClientRect()
{
	crect.x1 = pos.x+border;
	crect.y1 = pos.y+border+32;
	crect.x2 = pos.x+size.x-border;
	crect.y2 = pos.y+size.y-border;
}

void GUI2frame::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_panel);
	if(!mouseOver){setAlpha(128);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_border);
	setAlpha(255);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+32);
	setColor(color_text);
	printw(pos.x,pos.y+4, size.x, size.y,title);
	
	//glDisable(GL_SCISSOR_TEST);
}