#include "GUI.h"
#include "paint.h"
#include "simplemath.h"
#include "globals.h"
#include "selfaware.h"
#include "stringUtils.h"
#include "globals.h"

#define defaultarea rect(100,100)
#define defaultclientarea rect(100,100)
#define defaultbgcolor (vec3ub){128,196,255}
#define defaultborderColor (vec3ub){100,128,196}
#define defaulttextColor (vec3ub){0,0,0}
#define defaulttextfont getFont("calibri",18)
#define defaulttext "text"
#define defaultimage texture()
//#define defaultimage getTexture("resource/party.jpg")

//utility functions for GUI
GUIsetFavoriteRenderOptions(){
	setColor(defaultbgcolor);
	setAlpha(255);
	//firstn(50,printf("setAlpha\n"));
	clearScissor();
	setThickness(1);
	setTextMaxX(-1);
	setTextMaxY(-1);
	setTextPos({0,0});
	setInfiniteLine(false);
	setInfinitePlane(false);
	setFont(defaulttextfont);
	setTextScale(1);
}

//GUIbase
GUIbase::GUIbase(){
	parent = NULL;
	isClient = true;
	hidden = false;
	area = defaultarea;
	clientArea = defaultclientarea;
}

GUIbase &GUIbase::addChild(GUIbase &child){
	child.parent = this;
	children.push_back(&child);
	invalidate();
	child.invalidate();
	return *this;
}

GUIbase &GUIbase::setSize(vec2i newSize){
	area = area.setSize(newSize);
	invalidate();
	return *this;
}

GUIbase &GUIbase::moveTo(vec2i newstart){
	area = area.moveTo(newstart);
	//printf("moveTo: new area = %s\n",toString(area).c_str());
	invalidate();
	return *this;
}

GUIbase &GUIbase::setClientArea(rect newclientarea){
	clientArea = newclientarea;
	return *this;
}

GUIbase &GUIbase::setHidden(bool newHidden){
	hidden = newHidden;
	return *this;
}

GUIbase &GUIbase::sizeToContents(){
	int maxx = 0;
	int maxy = 0;
	for(int I = 0; I < children.size(); I++){
		maxx = max(maxx, children[I]->area.end.x);
		maxy = max(maxy, children[I]->area.end.y);
	}
	vec2i newend = {maxx,maxy};
	vec2i diff = newend - clientArea.end;
	clientArea.setSize(newend);
	area.setSize(area.size+diff);
	return *this;
}

void GUIbase::renderLogic(){
	pushRenderOptions();
	GUIsetFavoriteRenderOptions();
	for(int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			pushRenderOptions();
			children[I]->render();		
			children[I]->renderLogic();
			popRenderOptions();
		}
	}
	cropScissor(worldClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			pushRenderOptions();
			children[I]->render();		
			children[I]->renderLogic();
			popRenderOptions();
		}
	}
	popRenderOptions();
}

void GUIbase::render(){}

void GUIbase::onEvent(eventKind event){
	for(int I = 0; I < children.size(); I++){
		if(!children[I]->hidden){
			children[I]->onEvent(event);
		}
	}
}

void GUIbase::invalidate(){
	clientArea = rect().setStart({1,1}).setEnd(area.size-(vec2i){1,1});//rect(newarea.size);
}
//if something has coordinates L within
//the Area of this element,
//it was coordinates W in the world.
vec2i GUIbase::thisToWorld(vec2i L){
	if(parent){
		if(isClient){
			return (parent->clientToWorld(L))+area.start;
		}else{
			return (parent->thisToWorld(L))+area.start;
		}
	}else{
		return L+area.start;
	}
}

rect GUIbase::thisToWorld(rect L){
	return L.setStart(thisToWorld(L.start)).setEnd(thisToWorld(L.end));
}

//if something has coordinates L within
//the clientArea of this element,
//it was coordinates W in the world.
vec2i GUIbase::clientToWorld(vec2i L){
	if(parent){
		if(isClient){
			return (parent->clientToWorld(L))+area.start+clientArea.start;
		}else{
			return (parent->thisToWorld(L))+area.start+clientArea.start;
		}
	}else{
		return L+area.start+clientArea.start;
	}
}

rect GUIbase::clientToWorld(rect L){
	return L.setStart(clientToWorld(L.start)).setEnd(clientToWorld(L.end));
}

rect GUIbase::worldArea(){
	if(parent){
		return thisToWorld(rect(area.size));
	}else{
		vec2i scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::worldClientArea(){
	if(parent){
		return thisToWorld(clientArea);
	}else{
		vec2i scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::visibleArea(){
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			//return parent->worldClientArea().clamp(worldArea());
			return parent->visibleClientArea().clamp(worldArea());
		}else{
			return parent->visibleArea().clamp(worldArea());
		}
	}else{
		return worldArea();
	}
}
rect GUIbase::visibleClientArea(){
	if(hidden){return rect();}
	if(parent){
		if(isClient){
			//return parent->worldClientArea().clamp(worldArea());
			return parent->visibleClientArea().clamp(worldClientArea());
		}else{
			return parent->visibleArea().clamp(worldClientArea());
		}
	}else{
		return worldClientArea();
	}
}
//GUIframe
GUIframe::GUIframe(){
	bgColor = defaultbgcolor;
	borderColor = defaultborderColor;
}
GUIframe &GUIframe::setBgColor(vec3ub color){
	bgColor = color;
	return *this;
}

GUIframe &GUIframe::setBorderColor(vec3ub color){
	borderColor = color;
	return *this;
}

void GUIframe::render(){
	pushRenderOptions();
	GUIbase::render();
	setColor(bgColor);
	setAlpha(128);
	//drawRect(thisToWorld(rect(area.size)));
	drawRect(worldArea());
	setAlpha(255);
	setColor(borderColor);
	//drawRectOutline(thisToWorld(rect(area.size)));
	drawRectOutline(worldArea());
	popRenderOptions();
}

//GUIlabel
GUIlabel::GUIlabel(){
	textColor = defaulttextColor;
	textfont = defaulttextfont;
	text = defaulttext;
}
GUIlabel &GUIlabel::setTextColor(vec3ub color){
	textColor = color;
	return *this;
}
GUIlabel &GUIlabel::setTextFont(font f){
	textfont = f;
	return *this;
}
GUIlabel &GUIlabel::setText(string newtext){
	text = newtext;
	return *this;
}
GUIbase &GUIlabel::sizeToContents(){
	pushRenderOptions();
	GUIsetFavoriteRenderOptions();
	setFont(textfont);
	rect size = preprintw("%s",text.c_str());
	area = area.setSize(size.size);
	popRenderOptions();
	return *this;
}
void GUIlabel::render(){
	pushRenderOptions();
	//GUIframe::render();
	//GUIsetFavoriteRenderOptions();
	setColor(textColor);
	setFont(textfont);
	//vec2i pos = area.start;
	//pos.y += area.size.y/2;
	rect tRect = preprintw("%s",text.c_str());
	//setTextPos(thisToWorld(-tRect.start));
	setTextPos(worldArea().start-tRect.start);
	printw("%s",text.c_str());
	popRenderOptions();
}

//GUIimage
GUIimage::GUIimage(){
	image = defaultimage;
}
GUIimage &GUIimage::setImage(texture newimage){
	image = newimage;
	return *this;
}
GUIbase &GUIimage::sizeToContents(){
	area = area.setSize(image.size());
	return *this;
}
void GUIimage::render(){
	pushRenderOptions();
	GUIframe::render();
	setColor({255,255,255});
	setTexture(image);
	//drawTexturedRect(thisToWorld(rect(area.size)));
	drawTexturedRect(worldArea());
	popRenderOptions();
}

//GUIbutton
GUIbutton::GUIbutton(){
	pressed = false;
	mouseover = false;
	textColor = defaulttextColor;
	hoverColor = defaultbgcolor*0.9;
	pressedColor = defaultbgcolor*0.8;
	textfont = defaulttextfont;
	F = [](){return;};
	text = defaulttext;
	image = defaultimage;
}
GUIbutton &GUIbutton::setTextColor(vec3ub color){
	textColor = color;
	return *this;
}
GUIbutton &GUIbutton::setTextFont(font newfont){
	textfont = newfont;
	return *this;
}
GUIbutton &GUIbutton::setText(string newtext){
	text = newtext;
	return *this;
}
GUIbutton &GUIbutton::setImage(texture newimage){
	image = newimage;
	return *this;
}
GUIbutton &GUIbutton::setFunction(function<void()> f){
	F = f;
	return *this;
}
GUIbase &GUIbutton::sizeToContents(){
	vec2i size1 = image.size();
	pushRenderOptions();
	GUIsetFavoriteRenderOptions();
	setFont(textfont);
	rect R = preprintw("%s",text.c_str());
	//printf("sizeToContents:\nimage rect: %s\ntext rect: %s\n",
	//	toString(rect(size1)).c_str(),toString(R).c_str());
	vec2i size2 = R.size;
	popRenderOptions();
	area = area.setSize({max(size1.x,size2.x),max(size1.y,size2.y)});
	return *this;
}
void GUIbutton::render(){
	pushRenderOptions();
	vec3ub oldColor = bgColor;
	
	vec2i pos = getMousePos();
	//mouseover = thisToWorld(rect(area.size)).contains(pos);
	mouseover = visibleArea().contains(pos);
	if(!mouseover){pressed = false;}
	
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	setColor(bgColor);
	if(image.w() != 0){
		setTexture(image);
		//drawTexturedRect(thisToWorld(rect(area.size)));
		drawTexturedRect(worldArea());
	}
	setColor(textColor);
	setFont(textfont);
	//vec2i pos = area.start;
	//pos.y += area.size.y/2;
	rect tRect = preprintw("%s",text.c_str());
	vec2i offset;
	offset.x = (area.size.x - tRect.size.x)/2;
	offset.y = (area.size.y-tRect.size.y)/2-tRect.start.y;
	
	//setTextPos(thisToWorld(offset));
	setTextPos(worldArea().start+offset);
	printw("%s",text.c_str());
	popRenderOptions();
}
void GUIbutton::onEvent(eventKind event){
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){pressed = true;}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			F(); //once we call this... we might never return
			//jk but it might kill us (no member access from now on)
		}
	}
	/* 
	if(event.type == EVENT_MOUSE_MOVE){
		vec2i pos = getMousePos();
		mouseover = thisToWorld(rect(area.size)).contains(pos);
		if(!mouseover){pressed = false;}
	} */
}

//GUIscrollbarBar
GUIscrollbarBar::GUIscrollbarBar(){
	mouseover = false;
	pressed = false;
	offset = {0,0};
	hoverColor = bgColor*0.9;
	pressedColor = bgColor*0.8;
}

void GUIscrollbarBar::render(){
	pushRenderOptions();
	
	vec2i pos = getMousePos();
	//vec2i min = {parent->area.size.x-21,21};
	//vec2i max = {parent->area.size.x-21,parent->area.size.y-21-area.size.y};
	//if(pressed){moveTo(clamp(offset+pos, min,max));if(parent){parent->invalidate();}}
	//else{moveTo(clamp(area.start, min, max));}
	if(pressed){moveTo(offset+pos); if(parent){parent->invalidate();}}
	
	mouseover = visibleArea().contains(pos);
	//mouseover = thisToWorld(rect(area.size)).contains(pos);
	vec3ub oldColor = bgColor;
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	popRenderOptions();
}

void GUIscrollbarBar::onEvent(eventKind event){
	vec2i pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			offset = area.start-pos;
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){pressed = false;}
	}
}

//GUIscrollbar
GUIscrollbar::GUIscrollbar(){
	innerSize = area.size;
	//scrollPos = {0,0};
	bsizeToParent = false;
	vertical = true;
	horizontal = true;
	
	GUIbutton *btnUp = new GUIbutton();
	btnUp->isClient = false;
	btnUp->setText("^");
	btnUp->setSize({20,20});
	addChild(*btnUp);
	
	GUIbutton *btnDown = new GUIbutton();
	btnDown->isClient = false;
	btnDown->setText("v");
	btnDown->setSize({20,20});
	//btnDown->moveTo({0,20});
	addChild(*btnDown);
	
	GUIscrollbarBar *vbar = new GUIscrollbarBar();
	vbar->isClient = false;
	//vbar->setSize({20,30});
	addChild(*vbar);
	
	GUIbutton *btnLeft = new GUIbutton();
	btnLeft->isClient = false;
	btnLeft->setText("<");
	btnLeft->setSize({20,20});
	addChild(*btnLeft);
	
	GUIbutton *btnRight = new GUIbutton();
	btnRight->isClient = false;
	btnRight->setText(">");
	btnRight->setSize({20,20});
	addChild(*btnRight);
	
	GUIscrollbarBar *hbar = new GUIscrollbarBar();
	hbar->isClient = false;
	addChild(*hbar);
	
	
	btnUp->setFunction([=](){vbar->area = vbar->area.moveBy({0,-max(vbar->area.size.y/4,1)}); invalidate();});
	btnDown->setFunction([=](){vbar->area = vbar->area.moveBy({0,max(vbar->area.size.y/4,1)}); invalidate();});
	btnLeft->setFunction([=](){hbar->area = hbar->area.moveBy({-max(hbar->area.size.x/4,1),0}); invalidate();});
	btnRight->setFunction([=](){hbar->area = hbar->area.moveBy({max(hbar->area.size.x/4,1),0}); invalidate();});
	/* 
	GUIbutton *btnV = new GUIbutton();
	btnV->setText("vert");
	btnV->sizeToContents();
	btnV->moveTo({0,0});
	btnV->setFunction([this](){this->setVertical(!this->vertical);});
	addChild(*btnV);
	
	GUIbutton *btnH = new GUIbutton();
	btnH->setText("horiz");
	btnH->sizeToContents();
	btnH->moveTo({0,20});
	btnH->setFunction([this](){this->setHorizontal(!this->horizontal);});
	addChild(*btnH); */
	
	setVertical(vertical);
	setHorizontal(horizontal);
	invalidate();
}

GUIscrollbar &GUIscrollbar::setInnerSize(vec2i newInnerSize){
	innerSize = newInnerSize;
	invalidate();
	return *this;
}

GUIscrollbar &GUIscrollbar::setScrollOffset(vec2i newScrollOffset){	
	//printf("======setScrollOffset %s\n",toString(newScrollOffset).c_str());
	invalidate();
	float x =  (float)newScrollOffset.x / (innerSize.x-clientArea.size.x);
	float y =  (float)newScrollOffset.y / (innerSize.y-clientArea.size.y);
	setScrollRatio({x,y});
	invalidate();
	//printf("======new offset %s\n",toString(scrollOffset()).c_str());
	return *this;
}

// vec2f GUIscrollbar::scrollRatio(){
	// rect vtrack = vtrackRect();
	// rect htrack = htrackRect();
	// float xoffset = children[5]->area.start.x - htrack.start.x;
	// float x = (float)xoffset / (htrack.size.x-children[5]->area.size.x);
	// float yoffset = children[2]->area.start.y - vtrack.start.y;
	// float y = (float)yoffset / (vtrack.size.y-children[2]->area.size.y);
	// return {x,y};
// }
GUIscrollbar &GUIscrollbar::setScrollRatio(vec2f newScrollRatio){
	//printf("======setScrollRatio %s\n",toString(newScrollRatio).c_str());
	invalidate();
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = newScrollRatio.x * (htrack.size.x-children[5]->area.size.x);
	children[5]->area = children[5]->area.moveTo({xoffset + htrack.start.x,children[5]->area.start.y});
	float yoffset = newScrollRatio.y * (vtrack.size.y-children[2]->area.size.y);
	children[2]->area = children[2]->area.moveTo({children[2]->area.start.x,yoffset + vtrack.start.y});
	invalidate();
	//printf("======new ratio: %s\n",toString(scrollRatio()).c_str());
	return *this;
}

GUIscrollbar &GUIscrollbar::sizeToParent(bool b){
	bsizeToParent = b;
	return *this;
}

GUIscrollbar &GUIscrollbar::setVertical(bool newVertical){
	vec2i offset = scrollOffset();
	vertical = newVertical;
	if(children.size() >= 6){
		children[0]->setHidden(!vertical);
		children[1]->setHidden(!vertical);
		children[2]->setHidden(!vertical);
	}
	invalidate();
	setScrollOffset(offset);
	return *this;
}

GUIscrollbar &GUIscrollbar::setHorizontal(bool newHorizontal){
	vec2i offset = scrollOffset();
	horizontal = newHorizontal;
	if(children.size() >= 6){
		children[3]->setHidden(!horizontal);
		children[4]->setHidden(!horizontal);
		children[5]->setHidden(!horizontal);
	}
	invalidate();
	setScrollOffset(offset);
	return *this;
}

void GUIscrollbar::renderLogic(){
	vec2i offset = {0,0};
	if(children.size() >= 6){
		offset = scrollOffset();
	}
	pushRenderOptions();
	GUIsetFavoriteRenderOptions();
	for(int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			pushRenderOptions();
			children[I]->render();		
			children[I]->renderLogic();
			popRenderOptions();
		}
	}
	cropScissor(worldClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			pushRenderOptions();
			children[I]->area=children[I]->area.moveBy(-offset);
			children[I]->render();		
			children[I]->renderLogic();
			children[I]->area=children[I]->area.moveBy(offset);
			popRenderOptions();
		}
	}
	popRenderOptions();
}

void GUIscrollbar::render(){
	GUIframe::render();
	pushRenderOptions();
	setAlpha(128);
	setColor(bgColor*0.9);
	//rect R = rect().setStart({area.size.x-22,1}).setEnd({area.size.x-1,area.size.y-1});
	//drawRect(thisToWorld(R));
	if(vertical){
		rect R = vtrackRect().setStart(vtrackRect().start-(vec2i){0,21})\
					.setEnd(vtrackRect().end+(vec2i){0,21});
		drawRect(thisToWorld(R));
	}
	if(horizontal){
		rect R = htrackRect().setStart(htrackRect().start-(vec2i){21,0})\
					.setEnd(htrackRect().end+(vec2i){21,0});
		drawRect(thisToWorld(R));
	}
	popRenderOptions();
}

void GUIscrollbar::invalidate(){
	//firstn(10,printf("GUIscrollbar::invalidate, %d, %d, %d\n",(bool)parent,(bool)bsizeToParent,(children.size() >= 3)));
	if(parent && bsizeToParent){
		area = area.setSize(parent->clientArea.size);
	}
	clientArea = clientArea.setStart({1,1}).setEnd(area.size-(vec2i){vertical? 22: 0, horizontal? 22:0});
	if(children.size() >= 6){
		//children[0]->moveTo({area.end.x-21,area.start.y+1});
		//children[1]->moveTo({area.end.x-21,area.end.y-21});
		
		// children[0]->moveTo({area.size.x-21,1});
		// children[1]->moveTo({area.size.x-21,area.size.y-21});
		// float areaRatio = (float)clientArea.size.y/innerSize.y;
		// if(areaRatio > 1.0){areaRatio = 1.0;}
		// int scrollTrackHeight = area.size.y-42;
		// printf("track h = %d, inner h = %d, ratio = %.3f, final = %d\n",scrollTrackHeight,innerSize.y,areaRatio,scrollTrackHeight*areaRatio);
		// children[2]->setSize({20,scrollTrackHeight*areaRatio});
		rect vtrack = vtrackRect();
		children[0]->moveTo(vtrack.start-(vec2i){0,21});
		children[1]->moveTo(vtrack.end-children[1]->area.size+(vec2i){0,21});
		rect Vr = children[2]->area;
		Vr.size = {20,trackDimensions().y * areaRatio().y};
		Vr.size = clamp(Vr.size,{0,0},vtrack.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.end)-Vr.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.start));
		children[2]->area = Vr;
		
		rect htrack = htrackRect();
		children[3]->moveTo(htrack.start-(vec2i){21,0});
		children[4]->moveTo(htrack.end-children[4]->area.size+(vec2i){21,0});
		rect Hr = children[5]->area;
		Hr.size = {trackDimensions().x * areaRatio().x,20};
		Hr.size = clamp(Hr.size,{0,0},htrack.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.end)-Hr.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.start));
		children[5]->area = Hr;
		
		// printf("--------------\n");
		// printf("vtrackRect: %s\n",toString(vtrackRect()).c_str());
		// printf("htrackRect: %s\n",toString(htrackRect()).c_str());
		// printf("trackDimensions: %s\n",toString(trackDimensions()).c_str());
		// printf("areaRatio: %s\n",toString(areaRatio()).c_str());
		// printf("normalizedAreaRatio: %s\n",toString(normalizedAreaRatio()).c_str());
		// printf("scrollRatio: %s\n",toString(scrollRatio()).c_str());
		// printf("scrollOffset: %s\n",toString(scrollOffset()).c_str());
		
	}
}

rect GUIscrollbar::vtrackRect(){
	int height = trackDimensions().y;
	return rect().setStart({area.size.x-21,22}).setSize({20,height});
}

rect GUIscrollbar::htrackRect(){
	int width = trackDimensions().x;
	return rect().setStart({22,area.size.y-21}).setSize({width,20});
}

vec2i GUIscrollbar::trackDimensions(){
	return {vertical? area.size.x - 64 : area.size.x - 44, 
			horizontal? area.size.y - 64 : area.size.y - 44};
}

vec2f GUIscrollbar::areaRatio(){
	return {innerSize.y ? (float)clientArea.size.y/innerSize.y : 1.0,
			innerSize.x ? (float)clientArea.size.x/innerSize.x : 1.0};
}
vec2f GUIscrollbar::normalizedAreaRatio(){
	vec2f ar = areaRatio();
	ar.x = min(ar.x,1.0);
	ar.y = min(ar.y,1.0);
	return ar;
}

vec2f GUIscrollbar::scrollRatio(){
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = children[5]->area.start.x - htrack.start.x;
	float x = (float)xoffset / (htrack.size.x-children[5]->area.size.x);
	float yoffset = children[2]->area.start.y - vtrack.start.y;
	float y = (float)yoffset / (vtrack.size.y-children[2]->area.size.y);
	return {x,y};
}
//float -> integer round down causes x,y to drift towards 0
vec2i GUIscrollbar::scrollOffset(){
	int x = (innerSize.x-clientArea.size.x) * scrollRatio().x;
	int y = (innerSize.y-clientArea.size.y) * scrollRatio().y;
	return clamp({x,y},{0,0},innerSize-clientArea.size);
}

//GUIwindow
GUIwindow::GUIwindow(){
	hasCloseButton = true;
	moveable = true;
	pressed = false;
	mouseover = false;
	
	GUIbutton *btnClose = new GUIbutton();
	btnClose->isClient = false;
	btnClose->setSize({20,20});
	btnClose->setText({"x"});
	btnClose->setFunction([this](){
		printf("closebtn pressed\n");
		this->close();
	});
	addChild(*btnClose);
	
	GUIlabel *title = new GUIlabel();
	title->isClient = false;
	title->moveTo({5,5});
	title->setText({"Title"});
	addChild(*title);
	invalidate();
}

GUIwindow &GUIwindow::setCloseButton(bool hasbtn){
	children[0]->setHidden(!hasbtn);
	return *this;
}

GUIwindow &GUIwindow::setMoveable(bool newmoveable){
	moveable = newmoveable;
	return *this;
}

GUIwindow &GUIwindow::setTitle(string title){
	((GUIlabel*)children[1])->setText(title);
	((GUIlabel*)children[1])->sizeToContents();
	return *this;
}

void GUIwindow::close(){
	if(parent){
		auto &pc = parent->children;
		for(int I = 0; I < pc.size(); I++){
			if(pc[I] == this){
				pc.erase(pc.begin()+I);
				I--;
			}
		}
	}
}

void GUIwindow::render(){
	vec2i pos = getMousePos();
	mouseover = visibleArea().clamp(thisToWorld(rect(area.size.x-23,23))).contains(pos);
	if(pressed){moveTo(pos+offset);}
	
	pushRenderOptions();
	setColor(borderColor);
	drawRect(thisToWorld(rect(area.size.x,23)));
	popRenderOptions();
	GUIframe::render();
}

void GUIwindow::onEvent(eventKind event){
	vec2i pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			offset = area.start-pos;
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){pressed = false;}
	}
	if(!mouseover){
		GUIbase::onEvent(event);
	}
}

void GUIwindow::invalidate(){
	clientArea = area.setStart({0,23}).setEnd(area.size-(vec2i){1,1});
	if(children.size() >= 2){
		children[0]->moveTo({area.size.x-21,1});
	}
}

