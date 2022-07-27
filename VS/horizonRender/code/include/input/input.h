#ifndef INPUT_GUARD
#define INPUT_GUARD
#include "util/event.h"
//#include "input/keybinds.h"
//void initInput();
//struct gs_inputKind {
//	eventChannel* g_inputChannel;
//};

class sysInputKind {
public:
	sysInputKind();
	eventChannel inputChannel;
	//keybindList keybinds;
};

#endif