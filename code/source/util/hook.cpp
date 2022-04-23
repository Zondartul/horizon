#include "hook.h"
#include "stdio.h"
#include <vector>
using std::vector;

class hook:public eventListener{
	public:
	eventChannel *ch;
	eventType type;
	string name;
	function<void(eventKind event)> handler;
	void onEvent(eventKind event){
		if(event.type == type){handler(event);}
	}
};

vector<hook*> g_hooks;

void hookAdd(eventChannel *ch, eventType type, string name, function<void(eventKind event)> handler){
	hook *h = new hook();
	h->ch = ch;
	h->type = type;
	h->name = name;
	h->handler = handler;
	ch->addListener(h);
	g_hooks.push_back(h);
}

void hookRemove(eventChannel *ch, string name){
	for(auto I = g_hooks.begin(); I != g_hooks.end(); I++){
		if(((*I)->ch == ch) && ((*I)->name == name)){
			delete *I;
			I = g_hooks.erase(I);
		}
	}
}
