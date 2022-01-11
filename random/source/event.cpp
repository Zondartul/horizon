#include "event.h"
#include "globals.h"

void eventListenerList::publishEvent(eventKind e){
	for(int I = 0; I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
}

void eventListenerList::addListener(eventListener *listener){
	for(int I = 0; I < listeners.size(); I++){
		if(listeners[I] == listener){return;}
	}
	listeners.push_back(listener);
}

void eventListenerList::removeListener(eventListener *listener){
	for(int I = 0; I < listeners.size(); I++){
		if(listeners[I] = listener){
			listeners.erase(listeners.begin()+I);
			I--;
		}
	}
}

//removing this causes "undefined reference to vtable for <class>" error.
void eventListener::onEvent(eventKind event){}