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
	alignment_vertical = GUIa::Center;//ALIGN_BOTTOM;
	const_height = false;//true; //alignment doesn't work correctly with const_height
}

GUItextEntry *GUItextEntry::setText(string newtext){text = newtext; return this;}
//GUItextEntry *GUItextEntry::setCallOnEdit(bool b){callOnEdit = b; return this;}
//GUItextEntry *GUItextEntry::setCallOnEnter(bool b){callOnEnter = b; return this;}
//GUItextEntry *GUItextEntry::setMultiline(bool b){multiline = b; return this;}
//GUItextEntry *GUItextEntry::setNumeric(bool b){numeric = b; return this;}
GUItextEntry *GUItextEntry::setFunction(function<void()> f){F = f; return this;}
double GUItextEntry::getNumber(){float N = 0; sscanf(text.c_str(),"%f",&N); return N;}

//#define preserve(i,x) {auto temp = i; x; i = temp;}
/*
void GUItextEntry::render(){
	if(GUIoptions.push){pushRenderOptions();}
	vec3 oldColor1 = bgColor;
	vec3 oldColor2 = hoverColor;
	if(hasfocus){hoverColor = bgColor = focusedColor;}
	GUIbutton::render();
	if(hasfocus){
		rect TR;
		rect AR = worldArea();	
		//textOffset = vec2(0,0);
	textScrollLoop:
		if(cursorPos){
			TR = getTextRect(text.substr(0,cursorPos));
		}else{
			TR = getTextRect("l");
			TR.setSize(vec2(0,TR.size.y));
		}
		vec2 caretPos = AR.start+TR.end+textOffset;
		float diffRight = caretPos.x-AR.end.x+12.f;
		if(diffRight > 0){
			textOffset.x += -diffRight;
		}
		float diffLeft = AR.start.x+12.f-caretPos.x;
		if(diffLeft > 0){
			textOffset.x += diffLeft;
			//caretPos += textOffset;
		}
		if(textOffset.x > 0){textOffset.x = 0;}
		rect CR = tcaret->rect();
		caretPos.y = (AR.start.y+AR.end.y)/2.f-CR.size.y/2;
		caretPos.x -= CR.size.x*0.5f;
		rect IR = CR.moveBy(caretPos);
		setColor(vec3(255,255,255));
		drawImage(tcaret,IR);
		setTexturing(false);
		//setColor(vec3(255,0,0));
		//drawRectOutline(IR);
	}else{
		textOffset = vec2(0,0);
	}
	bgColor = oldColor1;
	hoverColor = oldColor2;
	if(GUIoptions.push){popRenderOptions();}
}
*/

void GUItextEntry::onEvent(eventKind event){
	GUIbase::onEvent(event);
	//if(event.isMasked()){return;}

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
			int C = event.keyboard.keycode;
			char pc = event.keyboard.printchar;
			int mods = event.keyboard.mod;
			//printf("key_down [key=%s]/[keycode=%d]\n",K,C);
			//printf("printchar = [%c], mod = [%d]\n",pc,mods);
			if(string("Backspace") == K){	//backspace
				//printf("isBP\n");
				event.maskEvent();
				if(text != ""){
					//printf("text popped\n");
					//text.pop_back();
					if(cursorPos){
						text.erase(cursorPos-1,1);
					}
				}//else{printf("not popped\n");}
				if(cursorPos){cursorPos--;}
				edit = true;
			}//else{printf("is not BP\n");}
			if(string("Return") == K){	//enter
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
				//printf("TE:enter:[%s]\n",text.c_str());
			}
			if(string("Left") == K){ // <- arrow
				if(cursorPos){cursorPos--;}
				event.maskEvent();
			}
			if(string("Right") == K){ // -> arrow
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
	//printf("text = [%s]\n",text.c_str());
	if(callOnEdit && edit){call = true;}
	if(callOnEnter && enter){call = true;}
	if(unfocusOnEnter && enter){unfocus = true;}
	if(callOnUnfocus && unfocus){call = true;}
	if(unfocus){hasfocus = false;}
	if(call){F();}
}

//property table
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
	//printf("%s::setProperty(%s)=[%s]\n",getType().c_str(),key.c_str(),val.c_str());

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
