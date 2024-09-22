#include "GUI/GUI_internal.h"
#include "util/global_vars_render.h"

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

	/// 25.01.2023: need to upload textures before using them
	auto& loadLayer = Gr->gs_paint->g_loadLayer;
	setLayer(loadLayer);
	uploadTexture(tcaret);
	/// 21.09.2024: add text selection
	select_begin = 0;
	select_end = 0;
	selecting = false;
}

GUItextEntry* GUItextEntry::setText(string newtext) { text = newtext; this->rT->text = newtext; return this; }
GUItextEntry *GUItextEntry::setFunction(function<void()> f){F = f; return this;}
double GUItextEntry::getNumber(){
	float N = 0; 
	stringstream ss(text);
	ss >> N;
	//sscanf(text.c_str(),"%f",&N); 
	return N;
}

string GUItextEntry::get_selection() {
	return text.substr(select_begin, select_end - select_begin);
}

void GUItextEntry::replace_selection(string rep) {
	if (select_begin == select_end) {return;}
	text = text.substr(0, select_begin) + rep + text.substr(select_end);//text.replace(select_begin, select_end, rep);
	select_end = select_begin;
	cursorPos = select_begin + rep.length();
	
	clear_selection();
}

void GUItextEntry::begin_selection() {
	if (!selecting) {
		select_begin = cursorPos;
		select_end = cursorPos;
		selecting = true;
	}
}

void GUItextEntry::clear_selection() {
	select_begin = 0;
	select_end = 0;
	selecting = false;
}

void GUItextEntry::select_left() {
	if (cursorPos < select_begin) {
		select_begin = cursorPos; // add a selected letter
	}else {
		select_end = cursorPos; // remove a selected letter
	}
}
void GUItextEntry::select_right() {
	if (cursorPos > select_end) {
		select_end = cursorPos; // add a selected letter
	}
	else {
		select_begin = cursorPos; // remove a selected letter
	}
}

void GUItextEntry::caret_left(bool select) {
	if (cursorPos) {
		if (select) {
			begin_selection();
			cursorPos--;
			select_left();
		}
		else{
			cursorPos--;
			clear_selection();
		}
	}
}

void GUItextEntry::caret_right(bool select) {
	if (cursorPos < text.length()) {
		if (select) {
			begin_selection();
			cursorPos++;
			select_right();
		}
		else {
			clear_selection();
			cursorPos++;
		}
	}
}

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
			bool shift_down = (event.keyboard.mod & MOD_SHIFT);
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
				replace_selection("");
				caret_left(false);
				edit = true;
			}
			if(string("Return") == K){	
				event.maskEvent();
				enter = true;

				if(multiline){
					//text += '\n';
					replace_selection("");
					text.insert(text.begin() + cursorPos, '\n');
					edit = true;
					cursorPos++;
				}
				if(clearOnEnter){
					text = "";
					edit = true;
				}
				clear_selection();
			}
			if(string("Left") == K){ 
				caret_left(shift_down);
				event.maskEvent();
			}
			if(string("Right") == K){ 
				caret_right(shift_down);
				event.maskEvent();
			}
			if(event.keyboard.printchar){
				event.maskEvent();
				replace_selection("");
				char C = event.keyboard.printchar;
				text.insert(text.begin()+cursorPos,C);
				edit = true;
				cursorPos++;
			}
			this->rT->text = text;
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
