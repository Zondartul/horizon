#include "keybinds.h"
#include "input.h"
#include "console.h"
keybindList::keybindList(){
	inputChannel->addListenerFront(this);
}
	
#define checkMB(x,y)							\
	if(event.mousebutton.button == x){			\
		if(binds.count(y)){						\
			event.maskEvent();					\
			console->run(binds[y].cmd);			\
		}										\
	}
	
#define checkKB(K)								\
	if(binds.count(K)){							\
		event.maskEvent();						\
		console->run(binds[K].cmd);				\
	}
	
void keybindList::onEvent(eventKind event){
	switch(event.type){
		case(EVENT_MOUSE_BUTTON_DOWN):
			checkMB(MOUSE_LEFT,		"+LMB");
			checkMB(MOUSE_RIGHT,	"+RMB");
			checkMB(MOUSE_MIDDLE,	"+MMB");
		break;
		case(EVENT_MOUSE_BUTTON_UP):
			checkMB(MOUSE_LEFT,		"-LMB");
			checkMB(MOUSE_RIGHT,	"-RMB");
			checkMB(MOUSE_MIDDLE,	"-MMB");
		break;
		case(EVENT_KEY_DOWN):
			checkKB(string("+")+event.keyboard.key);
		break;
		case(EVENT_KEY_UP):
			checkKB(string("-")+event.keyboard.key);
		break;
		case(EVENT_MOUSE_MOVE):
			checkKB("MouseMove");
		break;
	}
}

keybindList *keybinds;
void initKeybinds(){
	keybinds = new keybindList();
}

