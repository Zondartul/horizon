#include "gui/Gui2/GUI2radiogroup.h"
#include "gui/Gui2/GUI2radiobutton.h"
#include "display/paint.h"
#include "resource/fonts.h"
GUI2radiogroup::GUI2radiogroup()
{
	for(int I = 0;I<32;I++){buttons[I] = NULL;}
	selection = 0;
}

void GUI2radiogroup::addButton(void *btn)
{
	((GUI2radiobutton*)btn)->group = this;
	for(int I = 0;I<32;I++)
	{
		if(buttons[I]==NULL){buttons[I] = btn; return;}
	}
};
void GUI2radiogroup::checkButton(void *btn)
{
	((GUI2radiobutton*)btn)->checked = true;
	for(int I = 0;I<32;I++)
	{
		if(buttons[I]!=NULL)
		{
			if(buttons[I] != btn){((GUI2radiobutton*)(buttons[I]))->checked = false;}
			else{selection = I+1;}
		}
	}
};