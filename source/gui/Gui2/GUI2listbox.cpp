#include "gui/Gui2/GUI2listbox.h"
#include "display/paint.h"
GUI2listbox::GUI2listbox():GUI2base()
{
	size = {96,64};
	selected = 0;
	callback = NULL;
	callarg = NULL;
	selText = "";
	//color_panel = {196,196,196};
	for(int I = 0;I<32;I++)
	{
		sel[I] = NULL;
	}
}
void GUI2listbox::wrapFunc(void* arg)
{
	//arg:
	// 0 - I
	// 1 - self
	// 2 - prev arg
	// 3 - prev func
	void** newArg = (void**)arg;
	GUI2listbox* L = (GUI2listbox*)(newArg[1]);
	int I = *((int*)newArg[0]);
	L->selected = I+1;
	L->selText = L->sel[I]->text;
	if(L->callback){L->callback(L->callarg);}
	if(newArg[3]!=NULL)
	{
		void (*func)(void*) = (void (*)(void*))newArg[3];
		func(newArg[2]);
	}
}
void GUI2listbox::addOption(string text, void (*func)(void*), void* arg)
{
	for(int I = 0;I<32;I++)
	{
		if(sel[I]==NULL)
		{
			sel[I] = new GUI2button;
			void** newArg = new void*[4];
			int* aye;
			aye = new int;
			aye[0] = I;
			newArg[0] = (void*)aye;
			newArg[1] = (void*)this;
			newArg[2] = arg;
			newArg[3] = (void*)func;
			sel[I]->func = &wrapFunc;
			sel[I]->arg = (void*)newArg;
			sel[I]->text = text;
			sel[I]->size = {size.x, 14};
			sel[I]->setParent((GUI2base*)this);
			return;
		}
	}
}
void GUI2listbox::render(void* arg)
{
	resizeCheck();
	dragCheck();
	scissorCheck(arg);
	
	//setAlpha(128);
	//setColor(color_panel);
	//paintRect(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	//setAlpha(255);
	setColor(color_border);
	paintRectOutline(pos.x,pos.y,pos.x+size.x,pos.y+size.y);
	
	glDisable(GL_SCISSOR_TEST);
}

void GUI2listbox::invalidate(vec2i newPos, vec2i newSize)
{
	GUI2base::invalidate(newPos, newSize);
	for(int I = 0; I<32;I++)
	{
		if(sel[I]!=NULL)
		{
			sel[I]->setPos(pos.x,pos.y+14*I);
			sel[I]->setSize(size.x,14);
		}
	}
}

