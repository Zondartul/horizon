#include "gui/Gui2/GUI2valuedisplay.h"
#include "display/paint.h"
#include "resource/fonts.h"
GUI2valuedisplay::GUI2valuedisplay():GUI2base()
{
	size = {128,32};
	resizible = 0;
	precision = 2;
	movable = 0;
	val = NULL;
	mode = 'd';
}
void GUI2valuedisplay::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	if(val!=NULL)
	{
	switch(mode)
	{
		case 'd':
			sprintf(str,"%.*d",precision,*((int*)val));
			break;
		case 'f':
			sprintf(str,"%.*f",precision,*((double*)val));
			break;
		case 'c':
			sprintf(str,"%c",*((char*)val));
			break;
		case 's':
			sprintf(str,"%s",*((char**)val));
			break;
		case 'p':
			sprintf(str,"%p",*((void**)val));
			break;
		default:
			sprintf(str,"val:[%p]",val);
			break;
	}
	}else{sprintf(str,"val:[%p]",val);}
	printw(pos.x,pos.y,pos.x+size.x,pos.y+size.y,str);
	
	//glDisable(GL_SCISSOR_TEST);
}