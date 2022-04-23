#include "GUI_internal.h"

//GUIscrollbarBar
GUIscrollbarBar::GUIscrollbarBar(){
	scroll = 0;
	mouseover = false;
	pressed = false;
	offset = {0,0};
	hoverColor = bgColor*0.9;
	pressedColor = bgColor*0.8;
}

void GUIscrollbarBar::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}

	vec2 pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			event.maskEvent();
			offset = area.start-pos;
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_MOVE){
		vec2 pos = getMousePos();
		if(pressed){moveTo(offset+pos); if(parent){parent->invalidate();}}
	}
}

string GUIscrollbarBar::getType(){return "GUIscrollbarBar";}

//===========================================================================================

//GUIscrollbar
GUIscrollbar::GUIscrollbar(){
	innerSize = area.size;
	//scrollPos = {0,0};
	bSizeToParent = false;
	vertical = true;
	horizontal = true;
	offset = vec2(0,0);
	prevoffset = vec2(0,0);
    scrollingEnabled = true;

	
	inner = new GUIframe();
	inner->isClient = false;
	inner->setSize(innerSize);
	addChild(inner);
	//might also need frames for the tracks
	
	btnUp = new GUIbutton();
	btnUp->isClient = false;
	btnUp->setText("^");
	btnUp->setSize({20,20});
	addChild(btnUp);

	btnDown = new GUIbutton();
	btnDown->isClient = false;
	btnDown->setText("v");
	btnDown->setSize({20,20});
	addChild(btnDown);

	vbar = new GUIscrollbarBar();
	vbar->scroll = this;
	vbar->isClient = false;
	addChild(vbar);

	btnLeft = new GUIbutton();
	btnLeft->isClient = false;
	btnLeft->setText("<");
	btnLeft->setSize({20,20});
	addChild(btnLeft);

	btnRight = new GUIbutton();
	btnRight->isClient = false;
	btnRight->setText(">");
	btnRight->setSize({20,20});
	addChild(btnRight);

	hbar = new GUIscrollbarBar();
	hbar->scroll = this;
	hbar->isClient = false;
	addChild(hbar);

	btnUp->setFunction([=](){vbar->area = vbar->area.moveBy({0,-max(vbar->area.size.y/4,1)}); invalidate();});
	btnDown->setFunction([=](){vbar->area = vbar->area.moveBy({0,max(vbar->area.size.y/4,1)}); invalidate();});
	btnLeft->setFunction([=](){hbar->area = hbar->area.moveBy({-max(hbar->area.size.x/4,1),0}); invalidate();});
	btnRight->setFunction([=](){hbar->area = hbar->area.moveBy({max(hbar->area.size.x/4,1),0}); invalidate();});

	setVertical(vertical);
	setHorizontal(horizontal);
	constructed = true;
	invalidate();
}


GUIbase* GUIscrollbar::addChild(GUIbase *child){
	if(child->isClient){
		inner->addChild(child);
	}else{
		GUIframe::addChild(child);
	}
	return this;
}


void GUIscrollbar::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}
	//only deal with mouse scrolling here
	if(event.type == EVENT_MOUSE_WHEEL){
		event.maskEvent();
		int y = -20*event.mousewheel.y;
		if(vertical){
			setScrollOffset(vec2(offset.x,offset.y+y));
		}else if(horizontal){
			setScrollOffset(vec2(offset.x+y,offset.y));
		}
	}
}

void GUIscrollbar::clampInnerSizeToOuter(){
	printf("scroll:clampInnerSizeToOuter()\n");
	int max_x = max(area.size.x, inner->area.size.x);
	int max_y = max(area.size.y, inner->area.size.y);
	inner->setSize(vec2(max_x,max_y));
	innerSize = inner->area.size;
}

GUIscrollbar *GUIscrollbar::setSize(vec2 newSize){
	printf("scroll:setSize()\n");
	GUIframe::setSize(newSize);
	clampInnerSizeToOuter();
	invalidate();
	return this;
}

GUIscrollbar *GUIscrollbar::setInnerSize(vec2 newInnerSize){
	printf("scroll:setInnerSize()\n");
	inner->setSize(newInnerSize);
	clampInnerSizeToOuter();
	innerSize = inner->area.size;
	invalidate();
	return this;
}

GUIscrollbar *GUIscrollbar::sizeToContents(){
	printf("scroll::sizeToContents()\n");
	inner->sizeToContents();
	clampInnerSizeToOuter();
	invalidate();
	return this;
}

GUIscrollbar *GUIscrollbar::setScrollOffset(vec2 newScrollOffset){
	invalidate();
	float x =  (float)newScrollOffset.x / (innerSize.x-clientArea.size.x);
	float y =  (float)newScrollOffset.y / (innerSize.y-clientArea.size.y);
	setScrollRatio({x,y});
	invalidate();
	return this;
}

GUIscrollbar *GUIscrollbar::setScrollRatio(vec2 newScrollRatio){
	invalidate();
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = newScrollRatio.x * (htrack.size.x-hbar->area.size.x);
	hbar->area = hbar->area.moveTo({xoffset + htrack.start.x,hbar->area.start.y});
	float yoffset = newScrollRatio.y * (vtrack.size.y-vbar->area.size.y);
	vbar->area = vbar->area.moveTo({vbar->area.start.x,yoffset + vtrack.start.y});
	invalidate();
	return this;
}

GUIscrollbar *GUIscrollbar::sizeToParent(bool b){
	bSizeToParent = b;
	return this;
}

GUIscrollbar *GUIscrollbar::setVertical(bool newVertical){
	vec2 offset = scrollOffset();
	vertical = newVertical;
	if(children.size() >= 6){
		btnUp->setHidden(!vertical);
		btnDown->setHidden(!vertical);
		vbar->setHidden(!vertical);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

GUIscrollbar *GUIscrollbar::setHorizontal(bool newHorizontal){
	vec2 offset = scrollOffset();
	horizontal = newHorizontal;
	if(children.size() >= 6){
		btnLeft->setHidden(!horizontal);
		btnRight->setHidden(!horizontal);
		hbar->setHidden(!horizontal);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

GUIscrollbar *GUIscrollbar::enableScrolling(){
    scrollingEnabled = true;
    invalidate();
    return this;
}

GUIscrollbar *GUIscrollbar::disableScrolling(){
    scrollingEnabled = false;
    invalidate();
    return this;
}

void GUIscrollbar::renderLogic(){

	for(int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			if(g_GUIoptions.push){pushRenderOptions();}
			if(g_GUIoptions.debug){
				drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			}else{
				setScissoring(true);
				setScissor(visibleArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(g_GUIoptions.push){popRenderOptions();}
		}
	}

	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			if(g_GUIoptions.push){pushRenderOptions();}
			setScissoring(true);
			setScissor(visibleClientArea());
			
			children[I]->render();
			children[I]->renderLogic();
			if(g_GUIoptions.push){popRenderOptions();}
		}
	}
}


void GUIscrollbar::invalidate(){
	if(parent && bSizeToParent){
		area = area.setStart(vec2(0,0)).setSize(parent->clientArea.size);
	}else{
	}
	clientArea = clientArea.setStart({1,1}).setEnd(area.size-vec2{vertical? 22: 0, horizontal? 22:0});
	if(constructed){
		rect vtrack = vtrackRect();
		rect htrack = htrackRect();

		//put the up-button in it's place
		btnUp->moveTo(vtrack.start-vec2{0,21});

		//put the down-button in it's place
		btnDown->moveTo(vtrack.end-/*children[1]*/btnDown->area.size+vec2{0,21});

		//put the vertical scrollbar bar in it's place
		rect Vr = vbar->area;
		Vr.size = {20,trackDimensions().y * areaRatio().y};
		Vr.size = clamp(Vr.size,{0,0},vtrack.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.end)-Vr.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.start));
		vbar->area = Vr;

		//put the left-button in it's place
		btnLeft->moveTo(htrack.start-vec2{21,0});

		//put the right-button in it's place
		btnRight->moveTo(htrack.end-/*children[4]*/btnRight->area.size+vec2{21,0});

		//put the horizontal scrollbar bar in it's place
		rect Hr = hbar->area;
		Hr.size = {trackDimensions().x * areaRatio().x,20};
		Hr.size = clamp(Hr.size,{0,0},htrack.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.end)-Hr.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.start));
		hbar->area = Hr;
	}
	//displace children by offset
	//new (uses children on an inner frame)
	if(constructed){
		if(scrollingEnabled){ inner->moveTo( -scrollOffset() ); }
					    else{ inner->moveTo( vec2(0,0) ); }
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

vec2 GUIscrollbar::trackDimensions(){
	return {vertical? area.size.x - 64 : area.size.x - 44,
			horizontal? area.size.y - 64 : area.size.y - 44};
}

vec2 GUIscrollbar::areaRatio(){
	return {innerSize.x ? (float)clientArea.size.x/innerSize.x : 1.0,
			innerSize.y ? (float)clientArea.size.y/innerSize.y : 1.0};
}


vec2 GUIscrollbar::scrollRatio(){
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = hbar->area.start.x - htrack.start.x;
	float x = (float)xoffset / (htrack.size.x-hbar->area.size.x);
	float yoffset = vbar->area.start.y - vtrack.start.y;
	float y = (float)yoffset / (vtrack.size.y-vbar->area.size.y);
	return {x,y};
}
//float -> integer round down causes x,y to drift towards 0
vec2 GUIscrollbar::scrollOffset(){
	int x = (innerSize.x-clientArea.size.x) * scrollRatio().x;
	int y = (innerSize.y-clientArea.size.y) * scrollRatio().y;
	return clamp({x,y},{0,0},innerSize-clientArea.size);
}

	//property table
GUIpropertyTable GUIscrollbar::getDefaultPropertyTable(){
	GUIpropertyTable table = GUIframe::getDefaultPropertyTable();
	table.table["innerSize"] = toString(area.size);
	table.table["bSizeToParent"] = toString((bool)false);
	table.table["vertical"] = toString(bool(true));
	table.table["horizontal"] = toString(bool(true));
	table.table["scrollingEnabled"] = toString(bool(true));
	return table;
}

string GUIscrollbar::getProperty(string key){
	if(key == "innerSize"){return toString(innerSize);}
	else if(key == "bSizeToParent"){return toString(bSizeToParent);}
	else if(key == "vertical"){return toString(vertical);}
	else if(key == "horizontal"){return toString(horizontal);}
	else if(key == "scrollingEnabled"){return toString(scrollingEnabled);}
	else return GUIframe::getProperty(key);
}

void GUIscrollbar::setProperty(string key, string val){
	if(key == "innerSize"){setInnerSize(fromString<vec2>(val));}
	else if(key == "bSizeToParent"){bSizeToParent = fromString<bool>(val);}
	else if(key == "vertical"){setVertical(fromString<bool>(val));}
	else if(key == "horizontal"){setHorizontal(fromString<bool>(val));}
	else if(key == "scrollingEnabled"){
			if(fromString<bool>(val)){
				enableScrolling();
			}else{
				disableScrolling();
			}
	}else GUIframe::setProperty(key,val);
}

string GUIscrollbar::getType(){return "GUIscrollbar";}
