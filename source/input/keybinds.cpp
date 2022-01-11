#include "input/keybinds.h"
#include "util/globals.h"
#include "util/messenger.h"
#include "gui/newconsole.h"
#include "input/inputMessage.h"

map<string, string> KeyBinds;

void BindKind2::receiveMessage(message *msg){
	if((msg->type != "key_down") && (msg->type != "key_up"))
		return;
	message_key *msgk = (message_key*)msg;
	string key = msgk->key;
	if(KeyBinds.count(key)){
		if(msgk->type == "key_down"){
			parse(KeyBinds[key]);
		}else if(msgk->type == "key_up"){
			if(KeyBinds[key][0] == '+'){parse("-"+KeyBinds[key].substr(1,-1));}
			//else do not parse
		}
	}else{
		if(msgk->type == "key_down"){cprint("Unbound key: "+key);}
	}
}
BindKind2 Binder2;