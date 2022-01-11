#include "gui/Gui2/GUI2spinner.h"
#include "gui/Gui2/GUI2button.h"
#include "display/paint.h"
#include "resource/fonts.h"
void GUI2spinner::fUp(void* arg)
{
	double *vals = ((GUI2spinner*)arg)->vals;
	vals[1]+=vals[3];
	if(vals[1]>vals[2]){vals[1]=vals[2];}
	if(((GUI2spinner*)arg)->func){((GUI2spinner*)arg)->func(((GUI2spinner*)arg)->arg);}
}
void GUI2spinner::fDown(void* arg)
{
	double *vals = ((GUI2spinner*)arg)->vals;
	vals[1]-=vals[3];
	if(vals[1]<vals[0]){vals[1]=vals[0];}
	if(((GUI2spinner*)arg)->func){((GUI2spinner*)arg)->func(((GUI2spinner*)arg)->arg);}
}
GUI2spinner::GUI2spinner():GUI2base()
{
	counter = 0;
	size = {64,18};
	pos = {0,0};
	//vals = {-10,0,10,3,2};
	vals[0]=-10;vals[1]=0;vals[2]=10;vals[3]=3;vals[4]=2;
	movable = false;
	resizible = false;
	func = NULL;
	btnUp = new GUI2button;
	btnUp->func = &fUp;
	btnUp->arg = (void*)this;
	btnUp->size = {18,9};
	btnUp->resizible = false;
	btnUp->pos = {size.x-18,0};
	btnUp->text = "^";
	btnUp->setParent((GUI2base*)this);
	
	btnDown = new GUI2button;
	btnDown->func = &fDown;
	btnDown->arg = (void*)this;
	btnDown->size = {18,9};
	btnDown->resizible = false;
	btnDown->pos = {size.x-18,9};
	btnDown->text = "v";
	btnDown->setParent((GUI2base*)this);

}
void GUI2spinner::setVals(double a,double b,double c,double d,double e)
{
	vals[0]=a;vals[1]=b;vals[2]=c;vals[3]=d;vals[4]=e;
}
void GUI2spinner::invalidate(vec2i newPos, vec2i newSize)
{
	GUI2base::invalidate(newPos, newSize);
	btnUp->setPos(pos.x+size.x-18,pos.y);
	btnDown->setPos(pos.x+size.x-18,pos.y+9);
}
void GUI2spinner::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	
	setColor(color_panel);
	paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	setColor(color_text);
	printw(pos.x+2,pos.y,-1,-1,"%.*f", (int)vals[4], (double)vals[1]);
	
	//glDisable(GL_SCISSOR_TEST);
}