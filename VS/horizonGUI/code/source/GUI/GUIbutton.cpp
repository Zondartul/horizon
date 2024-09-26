#include "GUI/GUI_internal.h"

GUIbutton::GUIbutton(){
	pressed = false;
	mouseover = false;
	lastPressTime = 0;
	textColor = defaulttextColor;
	hoverColor = defaultbgcolor*0.9f;
	pressedColor = defaultbgcolor*0.8f;
	textfont = defaulttextfont;
	F = [](){return;};
	text = defaulttext;
	image = defaultimage;
	noframe = false;
}
GUIbutton *GUIbutton::setFunction(function<void()> f){
	F = f;
	return this;
}
GUIbase *GUIbutton::sizeToContents(){
	vec2 size1 = {0,0};
	if(image){size1 = image->size();}
	rect R = preprintw(textfont,"%s",text.c_str());
	vec2 size2 = R.size;
	area = area.setSize({max(size1.x,size2.x),max(size1.y,size2.y)});
	printf("GUIbutton:new area = %s\n",toString(area).c_str());
	return this;
}
void GUIbutton::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			event.maskEvent();
			lastPressTime = getRealTime();
			F(); 
			return;
		}
	}
}
GUIpropertyTable GUIbutton::getDefaultPropertyTable(){
	GUIpropertyTable table,table1,table2,table3;
	table1 = GUIframe::getDefaultPropertyTable();
	table2 = GUIimage::getDefaultPropertyTable();
	table3 = GUIlabel::getDefaultPropertyTable();
	table = table1+table2+table3;	
	table.table["hoverColor"] = toString(vec3(defaulttextColor));
	table.table["pressedColor"] = toString(vec3(defaultbgcolor*0.9f));
	return table;
}
string GUIbutton::getProperty(string key){
	if(key == "hoverColor"){return toString(hoverColor);}
	else if(key == "pressedColor"){return toString(pressedColor);}
	else{
		string S = GUIlabel::getProperty(key);
		if(S == ""){S = GUIimage::getProperty(key);}
		if(S == ""){S = GUIframe::getProperty(key);}
		return S;
	}
}
void GUIbutton::setProperty(string key, string val){
	if(key == "hoverColor"){hoverColor = fromString<vec3>(val).val();}
	else if(key == "pressedColor"){pressedColor = fromString<vec3>(val).val();}
	else{
		GUIlabel::setProperty(key,val);
		GUIimage::setProperty(key,val);
		GUIframe::setProperty(key,val);
	}
}
string GUIbutton::getType(){return "GUIbutton";}
