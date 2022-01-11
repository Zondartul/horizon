#include "frameprinter.h"
#include "globals.h"
#include "renderLayer.h"
#include "paint.h"
frameprinter *P;

void frameprint(string S){if(P){P->print(S);}else{error("no frameprinter");}}
void setFramePrinter(frameprinter *printer){P = printer;}

extern eventChannel globalChannel;
frameprinter::frameprinter(){globalChannel.addListener(this);}
frameprinter::~frameprinter(){globalChannel.removeListener(this);}
void frameprinter::print(string S){strings.push_back(S);}
void frameprinter::onEvent(eventKind event){if(event.type == EVENT_FRAME){render();}}

extern renderLayer *layer2D;
void frameprinter::render(){
	static renderLayer *layerFP=0;	//warning: shared among all instances of base class.
	if(layerFP){
		setLayer(layerFP);
	}else{
		layerFP = new renderLayer();
		setLayer(layer2D);
		addLayer(layerFP);
	}
	layerFP->clear();
	float Y = 0;
	setAlpha(255);
	for(auto I = strings.begin(); I != strings.end(); I++){
		setTextPos({0,Y});
		printText(*I);
		Y+=18;
	}
	strings.clear();
}