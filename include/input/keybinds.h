#ifndef KEYBINDS_GUARD
#define KEYBINDS_GUARD
#include "event.h"
#include <map>
#include <functional>
#include <string>
using std::map;
using std::function;
using std::string;

//a keybind list is a grouping of keybinds (key->function)
//that can be collectively turned on/off, or given priority.

struct bind{
	function<void(eventKind event)> keyDown;
	function<void(eventKind event)> keyUp;
};

class keybindList:public eventListener{
	public:
	map<string,bind> binds; //eventKind arg?
	keybindList();
	void onEvent(eventKind event);
};

extern keybindList *keybinds;
void initKeybinds();

#endif