#include "keybinds.h"
#include "input.h"

keybindList::keybindList(){
	inputChannel->addListenerFront(this);
}

#define checkMB(x,y,z)							\
if(event.mousebutton.button == x){				\
	if(binds.count(y) && binds[y].z != 0){		\
		event.maskEvent();						\
		binds[y].z(event);						\
	}											\
}

#define checkKB(K,x)							\
	if(binds.count(K) && binds[K].x != 0){		\
		event.maskEvent();						\
		binds[K].x(event);						\
	}						
	
void keybindList::onEvent(eventKind event){
	switch(event.type){
		case(EVENT_MOUSE_BUTTON_DOWN):
			checkMB(MOUSE_LEFT,		"LMB",keyDown);
			checkMB(MOUSE_RIGHT,	"RMB",keyDown);
			checkMB(MOUSE_MIDDLE,	"MMB",keyDown);
		break;
		case(EVENT_MOUSE_BUTTON_UP):
			checkMB(MOUSE_LEFT,		"LMB",keyUp);
			checkMB(MOUSE_RIGHT,	"RMB",keyUp);
			checkMB(MOUSE_MIDDLE,	"MMB",keyUp);
		break;
		case(EVENT_KEY_DOWN):
			checkKB(event.keyboard.key, keyDown);
		break;
		case(EVENT_KEY_UP):
			checkKB(event.keyboard.key, keyUp);
		break;
		case(EVENT_MOUSE_MOVE):
			checkKB("MouseMove", keyDown);
		break;
	}
}

keybindList *keybinds;
void initKeybinds(){
	keybinds = new keybindList();
}