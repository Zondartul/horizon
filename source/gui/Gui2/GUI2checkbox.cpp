#include "gui/Gui2/GUI2checkbox.h"
#include "display/paint.h"
#include "resource/fonts.h"
GUI2checkbox::GUI2checkbox():GUI2base()
{
	checked = false;
	resizible = false;
	movable = false;
	size = {14,14};
	func = NULL;
}

void GUI2checkbox::onClick(int mb)
{
	//GUIbase::onClick(mb);
	if((mb==0)&&mouseOver)
	{
		checked = !checked;
		if(func){func((void*)&checked);}
	}
}
void GUI2checkbox::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	if(focus==this){setAlpha(255);}else{setAlpha(196);}
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	if(checked){printw(pos.x+size.x/2-4,pos.y-4, -1, -1,"v");}
	glDisable(GL_SCISSOR_TEST);
}