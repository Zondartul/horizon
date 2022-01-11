#include "gui/Gui2/GUI2textEntry.h"
#include "display/paint.h"
#include "resource/fonts.h"
#include "main/control.h"
GUI2textEntry::GUI2textEntry():GUI2base()
{
	text = "Text entry";
	resizible = false;
	movable = false;
	multiline = false;
	size.y = 22;
	color_panel = {255,255,255};
	color_border = {0,0,0};
	color_text = {0,0,0};
	sizeToContents = false;
}
void GUI2textEntry::render(void* arg)
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
	if(sizeToContents){size.x = printw(pos.x+2,pos.y, -1, -1,text)+4;}
	else{printw(pos.x+2,pos.y, size.x-BRDB, size.y-BRDB,"%s", text.c_str())+4;}
	
	//glDisable(GL_SCISSOR_TEST);
}
void GUI2textEntry::onKeyboard(string kb)
{
	/*
	if(kb<0){return;}
	if(isprint(kb))
	{
		text += kb;printf("TE[0],");
	}
	
	if(kb==13)
	{
		if(multiline){text += '\n';}
	}
	if(kb==8)
	{
		if(text.length()){text.erase(text.length()-1);}
	}
	if(kb=='v')//ctrl+v = copy-paste
	{
		if(GetKeyState(VK_LCONTROL) & 0x8000)
		{
			text.erase(text.length()-1);
			OpenClipboard(NULL);
			HANDLE pasta = GetClipboardData(CF_TEXT);
			text += (char *)GlobalLock(pasta);
			GlobalUnlock(pasta);
			CloseClipboard();
		}
	}
	*/
	//cout << kb;
	if(kb == "backspace"){
		if(text.length()){text.erase(text.length()-1);}
	}else if((kb == "v") && input.keybuffer["ctrl"]){
		OpenClipboard(NULL);
		HANDLE pasta = GetClipboardData(CF_TEXT);
		text += (char *)GlobalLock(pasta);
		GlobalUnlock(pasta);
		CloseClipboard();
	}else if(kb == "enter"){
		if(multiline){text += '\n';}
	}else if(kb == "space"){
		text += " ";
	}else if(kb.length() == 1){
		if(input.keybuffer["shift"]){
			text += input.getShifted(kb[0]);
		}else{
			text += kb;
		}
	}
	//printf("TE[1],");
	if(callback){callback((void*)&kb);}
	
	//printf("TE[2],");
	
}