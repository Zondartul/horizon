#ifndef KEYBINDS_GUARD
#define KEYBINDS_GUARD
#include "event.h"
#include <map>
#include <functional>
#include <string>
using std::map;
using std::function;
using std::string;
struct bind{
	string cmd;
};
class keybindList:public eventListener{
	public:
	map<string,bind> binds; 
	keybindList();
	void onEvent(eventKind event);
	void checkMB(eventKind &event, int mb, string key);
	void checkKB(eventKind &event, string key);
};
void initKeybinds();
struct gs_keybindsKind {
	keybindList* g_keybinds;
};
#endif