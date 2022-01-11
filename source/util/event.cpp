#include "event.h"
#include "globals.h"

//todo: add event queuing capability

eventKind::eventKind(){mask = 0;} //so that it isn't implicitly deleted (probably cause of vec2 in a union)

void eventKind::maskEvent(int newmask){
	if(mask){*mask = newmask;}else{error("Event is non-maskable\n");}
}

int eventKind::isMasked(){
	if(mask && *mask){return *mask;}
	return 0;
}

//listeners can't mask events
//all listeners receive event
void eventListenerList::publishEvent(eventKind e){
	e.mask = 0;
	for(int I = 0; I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
}

//listeners can mask events
//a listener receives event only if it hasn't been masks
int eventListenerList::publishEventSequentialMaskable(eventKind e){
	int mask = 0;
	if(!e.mask){e.mask = &mask;}
	for(int I = 0; !mask && I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
	return *e.mask;
}

//listeners can mask events
//all listeners receive the event even if it is masked
int eventListenerList::publishEventParallelMaskable(eventKind e){
	int mask = 0;
	if(!e.mask){e.mask = &mask;}
	for(int I = 0; I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
	return *e.mask;
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

eventChannel *globalChannel;

void initEvents(){
	globalChannel = new eventChannel();
}