#include "gui/Gui2/GUI2dropdownlist.h"
#include "display/paint.h"
#include "resource/fonts.h"
void GUI2dropdownlist::showList(void* arg)
{
	GUI2dropdownlist *L = ((GUI2dropdownlist*)arg);
	if(!L->open){L->setSize(L->size.x,96);}else{L->setSize(L->size.x,14);}
	L->open = !L->open;
}
void GUI2dropdownlist::setCurOption(void* arg)
{
	GUI2dropdownlist* dd = (GUI2dropdownlist*)arg;
	dd->text = dd->list->selText;
	showList(dd);
}
GUI2dropdownlist::GUI2dropdownlist():GUI2base()
{
	open = false;
	movable = false;
	resizible = false;
	text = "";
	size = {64,14};
	list = new GUI2listbox;
	list->callback = &setCurOption;
	list->callarg = (void*)this;
	list->setParent((GUI2base*)this);
	btn = new GUI2button;
	btn->func = &showList;
	btn->arg = (void*)this;
	btn->setSize(14,14);
	btn->text = "<";
	btn->setParent((GUI2base*)this);
}
void GUI2dropdownlist::addOption(string text, void (*func)(void*), void* arg)
{
	list->addOption(text, func, arg);
}
void GUI2dropdownlist::render(void* arg)
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
	setColor(color_text);
	printw(pos.x-2,pos.y-2, size.x, size.y,text);
	
	glDisable(GL_SCISSOR_TEST);
}

void GUI2dropdownlist::invalidate(vec2i newPos, vec2i newSize)
{
	GUI2base::invalidate(newPos, newSize);
	btn->setPos(pos.x+size.x-14,pos.y);
	list->setPos(pos.x,pos.y+14);
	recalculateClientRect();
}