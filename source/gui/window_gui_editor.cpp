#include "gui/window_gui_editor.h"
#include "gui/Gui2.h"
#include "main/control.h"

void startGUIeditorSession(){
	openGUIeditorWindow();
}

GUI2frame *Window;

void openGUIeditorWindow(){
	Window = new GUI2frame();
	Window->setSize(400,300);
	Window->setPos(400,300);
	Window->title = "GUI Editor";
	Window->setParent(GUI);
	
	GUI2horizontalList *hl = new GUI2horizontalList();
	hl->setParent(Window);
	
	GUI2base *frame = new GUI2base();
	frame->setSize(200,300);
	frame->setParent(hl);
	
	GUI2base *frame2 = new GUI2base();
	frame2->setSize(200,300);
	frame2->setParent(hl);
}