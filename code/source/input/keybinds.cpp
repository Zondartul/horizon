#include "keybinds.h"
#include "input.h"
#include "console.h"
#include "global_vars.h"

keybindList::keybindList(){
	auto& inputChannel = G->gs_input->g_inputChannel;

	inputChannel->addListenerFront(this);
}

void keybindList::checkMB(eventKind &event, int mb, string key) {
	auto& console = G->gs_console->g_console;

	if (event.mousebutton.button == mb) {
		if (binds.count(key)) {
			event.maskEvent();
			console->run(binds[key].cmd);
		}
	}
}

//#define checkMB(x,y)							\
//	if(event.mousebutton.button == x){			\
//		if(binds.count(y)){						\
//			event.maskEvent();					\
//			g_console->run(binds[y].cmd);			\
//		}										\
//	}
//	
void keybindList::checkKB(eventKind& event, string key) {
	auto& console = G->gs_console->g_console;

	if (binds.count(key)) {
		event.maskEvent();
		console->run(binds[key].cmd);
	}
}

//#define checkKB(K)								\
//	if(binds.count(K)){							\
//		event.maskEvent();						\
//		g_console->run(binds[K].cmd);				\
//	}
	
void keybindList::onEvent(eventKind event){
	switch(event.type){
		case(EVENT_MOUSE_BUTTON_DOWN):
			checkMB(event, MOUSE_LEFT,		"+LMB");
			checkMB(event, MOUSE_RIGHT,	"+RMB");
			checkMB(event, MOUSE_MIDDLE,	"+MMB");
		break;
		case(EVENT_MOUSE_BUTTON_UP):
			checkMB(event, MOUSE_LEFT,		"-LMB");
			checkMB(event, MOUSE_RIGHT,	"-RMB");
			checkMB(event, MOUSE_MIDDLE,	"-MMB");
		break;
		case(EVENT_KEY_DOWN):
			checkKB(event, string("+")+event.keyboard.key);
		break;
		case(EVENT_KEY_UP):
			checkKB(event, string("-")+event.keyboard.key);
		break;
		case(EVENT_MOUSE_MOVE):
			checkKB(event, "MouseMove");
		break;
	}
}

//keybindList *g_keybinds;
void initKeybinds(){
	auto& keybinds = G->gs_keybinds->g_keybinds;

	keybinds = new keybindList();
}

