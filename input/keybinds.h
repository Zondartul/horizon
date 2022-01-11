#ifndef KEYBINDS_GUARD
#define KEYBINDS_GUARD
#include "../util/globals.h"
#include "../util/messenger.h"
extern map<string, string> KeyBinds;

class BindKind2: public PSsubscriber{
	void PSreceive(message msg);
};
extern BindKind2 Binder2;
#endif