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
	string cmd;
};


class keybindList:public eventListener{
	public:
	map<string,bind> binds; //eventKind arg?
	keybindList();
	void onEvent(eventKind event);
	void checkMB(eventKind &event, int mb, string key);
	void checkKB(eventKind &event, string key);
};

//extern keybindList *g_keybinds;
void initKeybinds();

struct gs_keybindsKind {
	keybindList* g_keybinds;
};

#endif