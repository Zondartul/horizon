#include "GUItextentry.h"


GUItextEntry::GUItextEntry(){
	//pressed = false;
	//mouseover = false;
	//textColor = defaulttextColor;
	//hoverColor = defaultbgcolor;
	//pressedColor = (vec3){255,255,255};
	//textfont = defaulttextfont;
	//F = [](){return;};
	//text = defaulttext;
	//image = defaultimage;
	focusedColor = (vec3){255,255,255};

	hasfocus = false;

	callOnEdit		= false;
	callOnEnter		= true;
	callOnUnfocus	= true;
	unfocusOnEnter	= true;
	clearOnEnter	= false;

	multiline = false;
	numeric = false;
	text = "";

	alignment_horizontal = ALIGN_LEFT;
	alignment_vertical = ALIGN_CENTER;//ALIGN_BOTTOM;
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
void GUItextEntry::render(){
	if(GUIoptions.push){pushRenderOptions();}
	vec3 oldColor1 = bgColor;
	vec3 oldColor2 = hoverColor;
	if(hasfocus){hoverColor = bgColor = focusedColor;}
	GUIbutton::render();
	bgColor = oldColor1;
	hoverColor = oldColor2;
	if(GUIoptions.push){popRenderOptions();}
}

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
			char C = event.keyboard.keycode;
			//printf("key_down [%s]/[%d]\n",K,C);
			if(string("Backspace") == K){	//backspace
				event.maskEvent();
				if(text != ""){text.pop_back();}
				edit = true;
			}
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
			if(event.keyboard.printchar){
				event.maskEvent();
				text += event.keyboard.printchar;
				edit = true;
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


