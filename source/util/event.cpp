#include "event.h"
#include "globals.h"

void eventKind::maskEvent(){
	if(mask){*mask = 1;}else{error("Event is non-maskable\n");}
}

void eventListenerList::publishEvent(eventKind e){
	e.mask = 0;
	for(int I = 0; I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
}

int eventListenerList::publishMaskableEvent(eventKind e){
	int mask = 0;
	e.mask = &mask;
	for(int I = 0; !mask && I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
	return mask;
}

void eventListenerList::addListener(eventListener *listener){
	for(int I = 0; I < listeners.size(); I++){
		if(listeners[I] == listener){return;}
	}
	listeners.push_back(listener);
}

void eventListenerList::addListenerFront(eventListener *listener){
	for(int I = 0; I < listeners.size(); I++){
		if(listeners[I] == listener){return;}
	}
	listeners.insert(listeners.begin(),listener);	
}

void eventListenerList::moveListenerToFront(eventListener *listener){
	removeListener(listener);
	addListenerFront(listener);
}

void eventListenerList::moveListenerToBack(eventListener *listener){
	removeListener(listener);
	addListener(listener);
}

void eventListenerList::removeListener(eventListener *listener){
	for(int I = 0; I < listeners.size(); I++){
		if(listeners[I] == listener){
			listeners.erase(listeners.begin()+I);
			I--;
		}
	}
}

//removing this causes "undefined reference to vtable for <class>" error.
void eventListener::onEvent(eventKind event){}