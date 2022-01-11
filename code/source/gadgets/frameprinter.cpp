#include "frameprinter.h"
#include "globals.h"
#include "renderLayer.h"
#include "paint.h"

//idea: make frameprinter save the last 10 seconds (600 frames) and cycle them.
//also make it have a decent GUI with openable categories that remembers
//which ones are open.
frameprinter *P;

void frameprint(string S){if(P){P->print(S);}/*else{error("no frameprinter");}*/}
void setFramePrinter(frameprinter *printer){P = printer;}

renderLayer *layerFP;
frameprinter::frameprinter(){
	layerFP = new renderLayer("frameprinter");
	layerFP->resetLayer = duplicateLayer(layer2D->resetLayer);
	layerFP->resetLayer->name = "frameprinter.reset";
	layerFP->resetLayer->special = true;
	addLayer(layer2D,layerFP);
	globalChannel->addListener(this);
}
frameprinter::~frameprinter(){globalChannel->removeListener(this);}
void frameprinter::print(string S){strings.push_back(S);}
void frameprinter::onEvent(eventKind event){if(event.type == EVENT_FRAME){render();}}

void frameprinter::render(){
	//static renderLayer *layerFP=0;	//warning: shared among all instances of base class.
	// if(layerFP){
		// setLayer(layerFP);
	// }else{
		// layerFP = new renderLayer();
		// setLayer(layer2D);
		// addLayer(layerFP);
	// }
	setLayer(layerFP);
	layerFP->clear();
	layerFP->reset();
	float Y = 14;
	setAlpha(255);
	setColor(vec3(255,255,255));
	for(auto I = strings.begin(); I != strings.end(); I++){
		setTextPos({0,Y});
		printText(string("[")+*I+"]");
		Y+=18;
	}
	strings.clear();
}