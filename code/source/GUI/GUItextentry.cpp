#include "GUI_internal.h"
GUItextEntry::GUItextEntry(){
	bgColor *= 1.1f;
	focusedColor = vec3{255,255,255};
	hasfocus = false;
	callOnEdit		= false;
	callOnEnter		= true;
	callOnUnfocus	= true;
	unfocusOnEnter	= true;
	clearOnEnter	= false;
	multiline = false;
	numeric = false;
	text = "";
	tcaret = getTexture("gui/caret12");
	cursorPos = 0;
	alignment_horizontal = GUIa::Left;
	alignment_vertical = GUIa::Center;
	const_height = false;
}
GUItextEntry *GUItextEntry::setText(string newtext){text = newtext; return this;}
GUItextEntry *GUItextEntry::setFunction(function<void()> f){F = f; return this;}
double GUItextEntry::getNumber(){float N = 0; sscanf(text.c_str(),"%f",&N); return N;}
void GUItextEntry::onEvent(eventKind event){
	GUIbase::onEvent(event);
	bool unfocus = 0;
	bool edit = 0;
	bool enter = 0;
	bool call = 0;
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			hasfocus = true;
		}else{
			if(hasfocus){unfocus = true;}
		}
	}
	if(hasfocus){
		if(event.type == EVENT_KEY_DOWN){
			const char *K = event.keyboard.key;
			//int C = event.keyboard.keycode;
			//char pc = event.keyboard.printchar;
			//int mods = event.keyboard.mod;
			if(string("Backspace") == K){	
				event.maskEvent();
				if(text != ""){
					if(cursorPos){
						text.erase(cursorPos-1,1);
					}
				}
				if(cursorPos){cursorPos--;}
				edit = true;
			}
			if(string("Return") == K){	
				event.maskEvent();
				enter = true;
				if(multiline){
					text += '\n';
					edit = true;
				}
				if(clearOnEnter){
					text = "";
					edit = true;
				}
			}
			if(string("Left") == K){ 
				if(cursorPos){cursorPos--;}
				event.maskEvent();
			}
			if(string("Right") == K){ 
				if(cursorPos < text.length()){cursorPos++;}
				event.maskEvent();
			}
			if(event.keyboard.printchar){
				event.maskEvent();
				char C = event.keyboard.printchar;
				text.insert(text.begin()+cursorPos,C);
				edit = true;
				cursorPos++;
			}
		}
	}
	if(callOnEdit && edit){call = true;}
	if(callOnEnter && enter){call = true;}
	if(unfocusOnEnter && enter){unfocus = true;}
	if(callOnUnfocus && unfocus){call = true;}
	if(unfocus){hasfocus = false;}
	if(call){F();}
}
GUIpropertyTable GUItextEntry::getDefaultPropertyTable(){
	GUIpropertyTable table = GUIbutton::getDefaultPropertyTable();
	table.table["focusedColor"] = toString(vec3({255,255,255}));
	table.table["callOnEdit"] = toString(bool(false));
	table.table["callOnEnter"] = toString(bool(true));
	table.table["callOnUnfocus"] = toString(bool(true));
	table.table["unfocusOnEnter"] = toString(bool(true));
	table.table["clearOnEnter"] = toString(bool(false));
	table.table["multiline"] = toString(bool(false));
	table.table["numeric"] = toString(bool(false));
	return table;
}
string GUItextEntry::getProperty(string key){
	if(key == "focusedColor"){return toString(focusedColor);}
	else if(key == "callOnEdit"){return toString(callOnEdit);}
	else if(key == "callOnEnter"){return toString(callOnEnter);}
	else if(key == "callOnUnfocus"){return toString(callOnUnfocus);}
	else if(key == "unfocusOnEnter"){return toString(unfocusOnEnter);}
	else if(key == "multiline"){return toString(multiline);}
	else if(key == "numeric"){return toString(numeric);}
	else return GUIbutton::getProperty(key);
}
void GUItextEntry::setProperty(string key, string val){
	if(key == "focusedColor"){focusedColor = fromString<vec3>(val);}
	else if(key == "callOnEdit"){callOnEdit = fromString<bool>(val);}
	else if(key == "callOnEnter"){callOnEnter = fromString<bool>(val);}
	else if(key == "callOnUnfocus"){callOnUnfocus = fromString<bool>(val);}
	else if(key == "clearOnEnter"){callOnEnter = fromString<bool>(val);}
	else if(key == "multiline"){multiline = fromString<bool>(val);}
	else if(key == "numeric"){numeric = fromString<bool>(val);}
	else GUIbutton::setProperty(key,val);
}
string GUItextEntry::getType(){return "GUItextEntry";}
