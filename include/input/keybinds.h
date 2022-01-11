#ifndef KEYBINDS_GUARD
#define KEYBINDS_GUARD
#include "../util/globals.h"
#include "../util/messenger.h"
extern map<string, string> KeyBinds;

class BindKind2: public messageReceiver{
	void receiveMessage(message *msg);
};
extern BindKind2 Binder2;
#endif