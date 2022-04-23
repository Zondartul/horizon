#include "frameprinter.h"
#include "globals.h"
#include "renderLayer.h"
#include "paint.h"

//idea: make frameprinter save the last 10 seconds (600 frames) and cycle them.
//also make it have a decent GUI with openable categories that remembers
//which ones are open.
frameprinter *g_P;

void frameprint(string S){if(g_P){g_P->print(S);}/*else{error("no frameprinter");}*/}
void setFramePrinter(frameprinter *printer){g_P = printer;}

renderLayer *layerFP;
frameprinter::frameprinter(){
	layerFP = new renderLayer("frameprinter");
	layerFP->resetLayer = duplicateLayer(g_layer2D->resetLayer);
	layerFP->resetLayer->name = "frameprinter.reset";
	layerFP->resetLayer->special = true;
	addLayer(g_layer2D,layerFP);
	g_globalChannel->addListener(this);
}
frameprinter::~frameprinter(){g_globalChannel->removeListener(this);}
void frameprinter::print(string S){strings.push_back(S);}
void frameprinter::onEvent(eventKind event){if(event.type == EVENT_FRAME){render();}}

void frameprinter::render(){
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