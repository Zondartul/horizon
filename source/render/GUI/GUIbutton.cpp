#include "GUIbutton.h"

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
/*
	//pushRenderOptions();
	vec3 oldColor = bgColor;

	vec2 pos = getMousePos();
	//mouseover = thisToWorld(rect(area.size)).contains(pos);
	mouseover = visibleArea().contains(pos);
	if(!mouseover){pressed = false;}

	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	setColor(bgColor);
	if(image){
		setTexture(image);
		//setTexturing(true); drawRect(thisToWorld(rect(area.size)));
		setTexturing(true);
		drawRect(worldArea());
		setTexturing(false);
	}
	setColor(textColor);
	setFont(textfont);
	//vec2 pos = area.start;
	//pos.y += area.size.y/2;
	rect tRect = preprintw(textfont,"%s",text.c_str());
	vec2 offset;
	offset.x = (area.size.x - tRect.size.x)/2;
	offset.y = (area.size.y-tRect.size.y)/2;

	//setTextPos(thisToWorld(offset));
	vec2 tp = getTextCentering(worldArea(),tRect);//+offset;
	//vec2 tpi = {tp.x,tp.y};
	//setColor({0,255,0});
	//drawRectOutline(tRect.moveBy(tp));
	setTextPos(vec2(tp.x,tp.y));
	printw("%s",text.c_str());
	//popRenderOptions();
*/
}
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
			F(); //once we call this... we might never return
			lastPressTime = getRealTime();
			//jk but it might kill us (no member access from now on)
		}
	}
	/*
	if(event.type == EVENT_MOUSE_MOVE){
		vec2 pos = getMousePos();
		mouseover = thisToWorld(rect(area.size)).contains(pos);
		if(!mouseover){pressed = false;}
	} */
}
