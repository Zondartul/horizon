#include "util/event.h"
#include "util/global_vars_render.h"
#include <cstdio>
#include <vector>
using std::vector;
//class hook:public eventListener{
//	public:
//	eventChannel *ch;
//	eventType type;
//	string name;
//	function<void(eventKind event)> handler;
//	void onEvent(eventKind event){
//		if(event.type == type){handler(event);}
//	}
//};

void hook::onEvent(eventKind event) {
	if (event.type == type) { handler(event); }
}

void sysEventKind::hookAdd(eventChannel *ch, eventType type, string name, function<void(eventKind event)> handler){
	//auto& hooks = Gb->gs_hook->g_hooks;
	hook *h = new hook();
	h->ch = ch;
	h->type = type;
	h->name = name;
	h->handler = handler;
	ch->addListener(h);
	hooks.push_back(h);
}
void sysEventKind::hookRemove(eventChannel *ch, string name){
	//auto& hooks = Gb->gs_hook->g_hooks;
	for (auto I = hooks.begin(); I != hooks.end(); I++) {
		if (((*I)->ch == ch) && ((*I)->name == name)) {
			delete* I;
			I = hooks.erase(I);
		}
	}
}

