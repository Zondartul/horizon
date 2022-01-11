#include "gui/Gui2/GUI2scrollBar.h"
#include "display/paint.h"
void GUI2scrollBar::btnupFunc(void *arg){
	GUI2scrollBar* S = (GUI2scrollBar*)arg;
	S->slidey->moveupdown(-S->slidey->size.y/2);
}
void GUI2scrollBar::btndnFunc(void *arg){
	GUI2scrollBar* S = (GUI2scrollBar*)arg;
	S->slidey->moveupdown(S->slidey->size.y/2);
}
void GUI2scrollBar::updateBar(void *arg)
{
	GUI2scrollBar* S = (GUI2scrollBar*)arg;
	
	if(S->vertical)
	{
		S->Amin = ((float)(S->slidey->pos.y-S->track->pos.y))/((float)(S->track->size.y));
		//S->Amax = ((float)(S->slidey->pos.y-S->track->pos.y+S->slidey->size.y+1))/((float)(S->track->size.y));
	}
	printf("Scroll: %f to %f ", S->Amin,S->Amax);
	foreach((GUI2base*)arg,&propagateScroll,arg,0);//'this' and 'arg' are NOT the same down the line.
	printf(" *\n");
}
int GUI2scrollBar::propagateScroll(GUI2base* obj, void* arg, int rec)
{
	GUI2scrollBar *S = (GUI2scrollBar*)arg;
	if((obj!=S->btnup)&&(obj!=S->btndn)&&(obj!=S->track))
	{
		vec2i newPos = obj->pos;
		
		if(S->vertical)
		{
			newPos.y = newPos.y-S->Aoffset; 
			S->Aoffset = -(S->Amin)*S->insideSize.y;
			//S->Aoffset = -(S->Amin/S->Amax)*S->insideSize.y;
			newPos.y = newPos.y+S->Aoffset;
		}
		else
		{
			newPos.x = newPos.x-S->Aoffset;
			S->Aoffset = -(S->Amin/S->Amax)*S->insideSize.x;
			newPos.x = newPos.x+S->Aoffset;
		}
		obj->setPos(newPos.x,newPos.y);
	}
}
void GUI2scrollBar::setSize(int x, int y)
{
	vec2i newSize = {x,y};
	if(parent){sizeOff = parent->size-newSize;}
	invalidate(pos, newSize);
	printf("newSize = {%d, %d}\n", newSize.x, newSize.y);
}
GUI2scrollBar::GUI2scrollBar():GUI2base()
{
	movable = false;
	resizible = false;
	size = {128,128};
	insideSize = size;
	insideSize.y += size.y;
	sizeOff = {0,0};
	vertical = true;
	Amin = 0.0;
	Amax = 0.5;
	Aoffset = 0;
	btnup = new GUI2button;
	btnup->setSize(16,16);
	btnup->text = "^";
	btnup->scissor = false;
	btnup->func = btnupFunc;
	btnup->arg = (void*)this;
	
	btndn = new GUI2button;
	btndn->setSize(16,16);
	btndn->text = "v";
	btndn->func = btndnFunc;
	btndn->arg = (void*)this;
	btndn->scissor = false; //for the future - use intrinsic children who use parent's scissor rect.
							//or, make "false scissor" do that...
	track = new GUI2button;
	track->scissor = false;
	color3i darken = {-64,-64,-64};
	track->color_panel.r *= 3;// + darken;
	track->color_panel.g *= 3;// + darken;
	track->color_panel.b *= 3;// + darken;
	track->color_panel.r /= 4;// + darken;
	track->color_panel.g /= 4;// + darken;
	track->color_panel.b /= 4;// + darken;
	
	
	slidey = new GUI2scrollslidey;
	slidey->setSize(16,32);
	slidey->scissor = false;
	slidey->callback = &updateBar;
	slidey->arg = (void*)this;
	
	btnup->setParent(this);
	btndn->setParent(this);
	track->setParent(this);
	slidey->setParent(track);
	slidey->setPos(track->pos.x,track->pos.y);
}
void GUI2scrollBar::setParent(GUI2base* obj)
{
	parent = obj;
	parent->recalculateClientRect();
	sizeOff = parent->size-size;
	printf("1 Psize = %d, size = %d, sizeOff = %d\n",parent->size.x, size.x, sizeOff.x);
	invalidate(pos+(vec2i){parent->crect.x1,parent->crect.y1}, size);
	GUI2base::addChild(parent, this);
	printf("2 Psize = %d, size = %d, sizeOff = %d\n",parent->size.x, size.x, sizeOff.x);
}
void GUI2scrollBar::invalidate(vec2i newPos, vec2i newSize)
{
	//this one should also reposition children and stuff
	vec2i sendPos = newPos-pos;//+newSize-size; and guess what happens when x is parented to y of size {0,0}?
	
	
	pos = newPos;
	recalculateClientRect();
	
	
	
	//size = newSize; //don't transmit size to chilren (use pack[2] if needed)
	
	if(parent&&(newSize==size)){size = parent->size-sizeOff;}
	else{size = newSize;}
	
	foreach(this, &wrapInvalidate, (void*)(&sendPos), 0);
	
	if(vertical)
	{
		btnup->setPos(pos.x+size.x-16,pos.y); //upper right
		btndn->setPos(pos.x+size.x-16,pos.y+size.y-15); //bottom right
		track->setSize(16,size.y-33);		
		track->setPos(pos.x+size.x-16,pos.y+17);
		//printf("tracksize = %d, size = %d, insideSize = %d, result = %d", track->size.y, size.y, insideSize.y,track->size.y*(size.y/insideSize.y));
		slidey->setSize(16,track->size.y*((float)(size.y-4)/(float)(insideSize.y)));//4 for border
	}
	else
	{
		btnup->setPos(pos.x,pos.y+size.y+2); //bottom left
		btndn->setPos(pos.x+size.x-15,pos.y+size.y+2); //bottom right
		track->setSize(size.x-33,16);		
		track->setPos(pos.x+18,pos.y+size.y+2);
		slidey->setSize(16,track->size.y*(size.y/insideSize.y));
	}
	
}
void GUI2scrollBar::render(void *arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x+1,pos.y+size.y+1);
	
	//draw

	//int X = pos.x;
	//float E = size.x;
	//int Yi = pos.y;
	//int Yf = pos.y+size.y;
	//vec3i col = {0,0,0};
	
	glDisable(GL_SCISSOR_TEST);
}