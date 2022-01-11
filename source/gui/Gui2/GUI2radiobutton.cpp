#include "gui/Gui2/GUI2radiobutton.h"
#include "display/paint.h"
#include "resource/fonts.h"
GUI2radiobutton::GUI2radiobutton():GUI2base()
{
	checked = false;
	resizible = false;
	movable = false;
	size = {16,16};
}
void GUI2radiobutton::onClick(int mb)
{
	if((mb==0)&&mouseOver&&(group!=NULL)){group->checkButton((void*)this);}
}
void GUI2radiobutton::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(focus==this){setAlpha(255);}else{setAlpha(196);}
	paintCircle(pos.x+size.x/2,pos.y+size.y/2,size.x/2);
	//paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setAlpha(255);
	setColor(color_border);
	paintCircleOutline(pos.x+size.x/2,pos.y+size.y/2,size.x/2);
	setColor(color_text);
	if(checked){printw(pos.x+size.x/2-5,pos.y-4, size.x, size.y,"o");}
	glDisable(GL_SCISSOR_TEST);
}