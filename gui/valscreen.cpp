#include "valscreen.h"
#include "../gui/Gui2.h"

GUI2label *valscreentext;

map<string,string> valscreenvals;

void valscreenrender(void *arg){
	valscreentext->text = "";
	for(std::map<string,string>::iterator I = valscreenvals.begin(); I != valscreenvals.end(); I++){
		valscreentext->text += I->first+":\n "+I->second+"\n";
	}
	if(valscreentext->text == "")
		valscreentext->text = "Nothing to display.";
}

void OpenValScreen(GUI2base *GUI){
	GUI2frame *valscreen = new GUI2frame;
	valscreen->setPos(32,375);
	valscreen->setSize(400,200);
	valscreen->setParent(GUI);
	
	GUI2scrollBar *scroll = new GUI2scrollBar;
	scroll->setSize(400-BRDB,200-BRDT-1);
	scroll->insideSize.y = 2000;
	scroll->setParent(valscreen);
	
	valscreentext = new GUI2label;
	valscreentext->setSize(400-2*BRDB-16,2000);
	valscreentext->setParent(scroll);
	
	valscreentext->think = valscreenrender;
};

void printvals(string str1, string str2){
	valscreenvals[str1] = str2;
}