#include "input/keybinds.h"
#include "util/globals.h"
#include "util/messenger.h"
#include "gui/newconsole.h"

map<string, string> KeyBinds;

void BindKind2::PSreceive(message msg){
	if((msg.type != "key_down") && (msg.type != "key_up"))
		return;
	//msgintercept(msg);
	string key = msg.str;
	if(KeyBinds.count(key)){
		if(msg.type == "key_down")
			parse(KeyBinds[key]);
		else if(msg.type == "key_up"){
			if(KeyBinds[key][0] == '+')
				parse("-"+KeyBinds[key].substr(1,-1));
			//else do not parse
		}
	}else
		if(msg.type == "key_down")
		cprint("Unbound key: "+key);
}
BindKind2 Binder2;