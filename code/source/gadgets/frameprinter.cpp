#include "frameprinter.h"
#include "globals.h"
#include "renderLayer.h"
#include "paint.h"

//idea: make frameprinter save the last 10 seconds (600 frames) and cycle them.
//also make it have a decent GUI with openable categories that remembers
//which ones are open.
//frameprinter *g_P;

void frameprint(string S){
	auto& P = G->gs_frameprinter->g_P;

	if(P){P->print(S);}
}
void setFramePrinter(frameprinter *printer){
	auto& P = G->gs_frameprinter->g_P;
	P = printer;
}

//renderLayer *g_layerFP;
frameprinter::frameprinter(){
	auto& layerFP = G->gs_frameprinter->g_layerFP;
	auto& layer2D = G->gs_paint->g_layer2D;
	auto& globalChannel = G->gs_event->g_globalChannel;

	layerFP = new renderLayer("frameprinter");
	layerFP->resetLayer = duplicateLayer(layer2D->resetLayer);
	layerFP->resetLayer->name = "frameprinter.reset";
	layerFP->resetLayer->special = true;
	addLayer(layer2D,layerFP);
	globalChannel->addListener(this);
}
frameprinter::~frameprinter(){
	auto& globalChannel = G->gs_event->g_globalChannel;

	globalChannel->removeListener(this);
}
void frameprinter::print(string S){strings.push_back(S);}
void frameprinter::onEvent(eventKind event){if(event.type == EVENT_FRAME){render();}}

void frameprinter::render(){
	auto& layerFP = G->gs_frameprinter->g_layerFP;

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