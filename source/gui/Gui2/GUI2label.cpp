#include "gui/Gui2/GUI2label.h"
#include "resource/fonts.h"
GUI2label::GUI2label():GUI2base()
{
	text = "label";
	resizible = false;
	movable = false;
	size.y = 22;
	size.x = 256;
}
void GUI2label::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	int defcolor[3];
	defcolor[0] = color_text.r;
	defcolor[1] = color_text.g;
	defcolor[2] = color_text.b;
	printwrich(pos.x,pos.y, size.x, size.y, defcolor, text);
	//printw(pos.x,pos.y,size.x,size.y,"print error");
	//size.x = printw;
	//glDisable(GL_SCISSOR_TEST);
}