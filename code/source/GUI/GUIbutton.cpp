#include "GUI_internal.h"

//GUIbutton
GUIbutton::GUIbutton(){
	pressed = false;
	mouseover = false;
	lastPressTime = 0;
	textColor = defaulttextColor;
	hoverColor = defaultbgcolor*0.9;
	pressedColor = defaultbgcolor*0.8;
	textfont = defaulttextfont;
	F = [](){return;};
	text = defaulttext;
	image = defaultimage;
	noframe = false;
}
/*
GUIbutton *GUIbutton::setTextColor(vec3 color){
	textColor = color;
	return this;
}
GUIbutton *GUIbutton::setTextFont(font *newfont){
	textfont = newfont;
	return this;
}
GUIbutton *GUIbutton::setText(string newtext){
	text = newtext;
	return this;
}
GUIbutton *GUIbutton::setImage(texture *newimage){
	image = newimage;
	return this;
}
*/
GUIbutton *GUIbutton::setFunction(function<void()> f){
	F = f;
	return this;
}
GUIbase *GUIbutton::sizeToContents(){
	vec2 size1 = {0,0};
	if(image){size1 = image->size();}
	rect R = preprintw(textfont,"%s",text.c_str());
	//printf("sizeToContents:\nimage rect: %s\ntext rect: %s\n",
	//	toString(rect(size1)).c_str(),toString(R).c_str());
	vec2 size2 = R.size;
	//popRenderOptions();
	area = area.setSize({max(size1.x,size2.x),max(size1.y,size2.y)});
	printf("GUIbutton:new area = %s\n",toString(area).c_str());
	return this;
}

/*
void GUIbutton::render(){
	if(GUIoptions.push){pushRenderOptions();}
	vec3 oldColor = bgColor;
	if(!mouseover){pressed = false;}
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;

	if(image){GUIimage::render();}
	GUIlabel::render();
	if(GUIoptions.push){popRenderOptions();}

}
*/

void GUIbutton::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}

	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			//printf("btn click\n");
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			//printf("btn unclick\n");
			event.maskEvent();
			lastPressTime = getRealTime();
			F(); //once we call this... we might never return
			//jk but it might kill us (no member access from now on)
			return;
		}
	}
	/*
	if(event.type == EVENT_MOUSE_MOVE){
		vec2 pos = getMousePos();
		mouseover = thisToWorld(rect(area.size)).contains(pos);
		if(!mouseover){pressed = false;}
	} */
}

GUIpropertyTable GUIbutton::getDefaultPropertyTable(){
	GUIpropertyTable table,table1,table2,table3;
	table1 = GUIframe::getDefaultPropertyTable();
	table2 = GUIimage::getDefaultPropertyTable();
	table3 = GUIlabel::getDefaultPropertyTable();

	table = table1+table2+table3;	
	table.table["hoverColor"] = toString(vec3(defaulttextColor));
	table.table["pressedColor"] = toString(vec3(defaultbgcolor*0.9));
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
	//printf("%s::setProperty(%s)=[%s]\n",getType().c_str(),key.c_str(),val.c_str());

	if(key == "hoverColor"){hoverColor = fromString<vec3>(val);}
	else if(key == "pressedColor"){pressedColor = fromString<vec3>(val);}
	else{
		GUIlabel::setProperty(key,val);
		GUIimage::setProperty(key,val);
		GUIframe::setProperty(key,val);
	}
}


string GUIbutton::getType(){return "GUIbutton";}










