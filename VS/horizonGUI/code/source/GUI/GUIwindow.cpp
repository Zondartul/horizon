#include "GUI/GUI_internal.h"

GUIwindow::GUIwindow(){
	hasCloseButton = true;
	moveable = true;
	pressed = false;
	mouseover = false;
	mouseovertitle = false;
	btnClose = new GUIbutton();
	btnClose->isClient = false;
	btnClose->setSize({21,21});
	btnClose->setText({"x"});
	btnClose->setFunction([this](){
		printf("closebtn pressed\n");
		this->close();
	});
	addChild(btnClose);
	lblTitle = new GUIlabel();
	lblTitle->isClient = false;
	lblTitle->moveTo({5,5});
	lblTitle->sizeToContents();
	addChild(lblTitle);
	setTitle("Title");
	invalidate();
}
GUIwindow *GUIwindow::setCloseButton(bool hasbtn){
	btnClose->setHidden(!hasbtn);
	hasCloseButton = hasbtn;
	return this;
}
GUIwindow *GUIwindow::setMoveable(bool newmoveable){
	moveable = newmoveable;
	return this;
}
GUIwindow *GUIwindow::setTitle(string title){
	lblTitle->setText(title);
	lblTitle->sizeToContents();
	this->title = title;
	return this;
}
void GUIwindow::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}
	vec2 pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			printf("click window\n");
			event.maskEvent();
			if(mouseovertitle){
				pressed = true;
				offset = area.start-pos;
			}
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			printf("unclick window\n");
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_MOVE){
		vec2 pos = getMousePos();
		mouseovertitle = visibleArea().clamp(thisToWorld(rect(area.size.x-23,23))).contains(pos);
		if(pressed && moveable){moveTo(pos+offset);} 
	}
}
void GUIwindow::invalidate(){
	clientArea = area.setStart({0,23}).setEnd(area.size-vec2{1,1});
	if(children.size() >= 2){
		children[0]->moveTo({area.size.x-(1+children[0]->area.size.x),1});
	}
}
GUIpropertyTable GUIwindow::getDefaultPropertyTable(){
	GUIpropertyTable table = GUIframe::getDefaultPropertyTable();
	table.table["hasCloseButton"] = toString(bool(true));
	table.table["moveable"] = toString(bool(true));
	table.table["title"] = "Title";
	return table;
}
string GUIwindow::getProperty(string key){
	if(key == "hasCloseButton"){return toString(hasCloseButton);}
	else if(key == "moveable"){return toString(moveable);}
	else if(key == "title"){return toString(title);}
	else return GUIframe::getProperty(key);
}
void GUIwindow::setProperty(string key, string val){
	if(key == "hasCloseButton"){setCloseButton(fromString<bool>(val));}
	else if(key == "moveable"){moveable = fromString<bool>(val);}
	else if(key == "title"){setTitle(val);}
	else GUIframe::setProperty(key,val);
}
string GUIwindow::getType(){return "GUIwindow";}