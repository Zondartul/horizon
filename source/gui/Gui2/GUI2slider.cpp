#include "gui/Gui2/GUI2slider.h"
#include "display/paint.h"
#include "resource/fonts.h"
#include "main/control.h"
GUI2slider::GUI2slider():GUI2base()
{
	movable = false;
	resizible = false;
	color_panel = {128,128,256};
	size = {128,14};
	vals = {-100,0,100};
	//vals[0]=-100;vals[1]=0;vals[2]=100;
	sliding = false;
}
void GUI2slider::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	updateSlider();
	//setAlpha(128);
	//setColor(color_panel);
	//paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	//setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y+size.y/2,pos.x+size.x,pos.y+size.y/2);
	setColor(color_panel);
	double xdot = (int)(size.x*((vals[1]-vals[0])/(vals[2]-vals[0])));
	paintCircle(pos.x+xdot,pos.y+size.y/2,size.y/2);
	setColor(color_border);
	paintCircleOutline(pos.x+xdot,pos.y+size.y/2,size.y/2);
	setColor(color_text);
	printw(pos.x+size.x,pos.y, size.x, size.y,"%f",vals[1]);
	glDisable(GL_SCISSOR_TEST);
}
void GUI2slider::onClick(int mb)
{
	sliding = mb;
}
void GUI2slider::updateSlider()
{
	if(sliding)
	{
		vals[1] = (mouseP.x-pos.x)*(vals[2]-vals[0])/size.x+vals[0];
		vals[1] = (vals[1] < vals[0]) ? vals[0] : ((vals[1] > vals[2]) ? vals[2] : vals[1]);
	}
}