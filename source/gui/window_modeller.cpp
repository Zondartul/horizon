#include "gui/window_modeller.h"
#include "gui/Gui2.h"
#include "main/control.h"

GUI2frame *frame;
vector<GUI2button*> buttons;

void OpenWindowModeller(){
	frame = new GUI2frame;
	frame->setPos(64,64);
	frame->setSize(64+4,512);
	frame->title = "Modeller";
	frame->setParent(GUI);
	
	for(int I = 0; I<12; I++){
		GUI2button *btn = new GUI2button;
		btn->setPos(0,0+32*I);
		btn->setSize(32,32);
		btn->func = NULL;
		btn->arg = NULL;
		btn->setParent((GUI2base*) frame);
		buttons.push_back(btn);
		
		GUI2button *btn2 = new GUI2button;
		btn2->setPos(32+0,0+32*I);
		btn2->setSize(32,32);
		btn2->func = NULL;
		btn2->arg = NULL;
		btn2->setParent((GUI2base*) frame);
		buttons.push_back(btn2);
	}
	buttons[0]->setImage("../resource/textures/gui/iconnew.png");
	buttons[1]->setImage("../resource/textures/gui/iconmove.png");
	buttons[2]->setImage("../resource/textures/gui/iconopen.png");
	buttons[3]->setImage("../resource/textures/gui/iconscale.png");
}

/* this is from toolbox
	GUI2frame* toolFrame = new GUI2frame;
	toolFrame->setPos(64,64);
	toolFrame->setSize(64+8,256);
	toolFrame->title = "Toolbox";
	toolFrame->setParent(GUI);
	
	GUI2button* btnBox = new GUI2button;						GUI2button* btnCyl = new GUI2button;
	btnBox->setPos(4,4);										btnCyl->setPos(4+32,4);
	btnBox->setSize(32,32);										btnCyl->setSize(32,32);
	btnBox->setImage("C:/Stride/textures/cube.bmp");			btnCyl->setImage("C:/Stride/textures/cylinder.bmp");
	btnBox->func = &genBox;										btnCyl->func = &genCyl;
	btnBox->arg = NULL;											btnCyl->arg = NULL;
	btnBox->setParent((GUI2base*) toolFrame);					btnCyl->setParent((GUI2base*) toolFrame);
	*/