#include "event.h"
#include "globals.h"

//todo: add event queuing capability

eventKind::eventKind(){mask = 0;} //so that it isn't implicitly deleted (probably cause of vec2 in a union)

void eventKind::maskEvent(int newmask){
	if(mask){*mask = newmask;}else{error("Event is non-maskable (%s)\n",toString(type).c_str());}
}

int eventKind::isMasked(){
	if(mask && *mask){return *mask;}
	return 0;
}

string toString(eventType et){
	switch(et){
		case EVENT_KEY_UP: 				return "EVENT_KEY_UP";
		case EVENT_KEY_DOWN: 			return "EVENT_KEY_DOWN";
		case EVENT_MOUSE_BUTTON_UP: 	return "EVENT_MOUSE_BUTTON_UP";
		case EVENT_MOUSE_BUTTON_DOWN:	return "EVENT_MOUSE_BUTTON_DOWN";
		case EVENT_MOUSE_WHEEL:			return "EVENT_MOUSE_WHEEL";
		case EVENT_FRAME: 				return "EVENT_FRAME";
		case EVENT_TICK:				return "EVENT_TICK";
		case EVENT_MOUSE_MOVE: 			return "EVENT_MOUSE_MOVE";
		default:						return "<ERROR>";
	}
}

//listeners can't mask events
//all listeners receive event
void eventListenerList::publishEvent(eventKind e){
	e.mask = 0;
	for(unsigned int I = 0; I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
}

//listeners can mask events
//a listener receives event only if it hasn't been masks
int eventListenerList::publishEventSequentialMaskable(eventKind e){
	int mask = 0;
	if(!e.mask){e.mask = &mask;}
	for(unsigned int I = 0; !mask && I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
	return *e.mask;
}

//listeners can mask events
//all listeners receive the event even if it is masked
int eventListenerList::publishEventParallelMaskable(eventKind e){
	//this can fail mid-way as listeners[I]->onEvent() may delete us.
	int mask = 0;
	if(!e.mask){e.mask = &mask;}
	for(unsigned int I = 0; I < listeners.size(); I++){
		listeners[I]->onEvent(e);
	}
	return *e.mask;
}

void eventListenerList::addListener(eventListener *listener){
	for(unsigned int I = 0; I < listeners.size(); I++){
		if(listeners[I] == listener){return;}
	}
	listeners.push_back(listener);
	listener->channels.push_back(this);
}

void eventListenerList::addListenerFront(eventListener *listener){
	for(unsigned int I = 0; I < listeners.size(); I++){
		if(listeners[I] == listener){return;}
	}
	listeners.insert(listeners.begin(),listener);
	listener->channels.push_back(this);
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
	for(unsigned int I = 0; I < listeners.size(); I++){
		if(listeners[I] == listener){
			listeners.erase(listeners.begin()+I);
			I--;
		}
	}
}

//removing this causes "undefined reference to vtable for <class>" error.
void eventListener::onEvent(eventKind event){}

eventListener::~eventListener(){
	for(auto I = channels.begin(); I != channels.end(); I++){
		(*I)->removeListener(this);
	}
}

eventChannel *g_globalChannel;

void initEvents(){
	g_globalChannel = new eventChannel();
}