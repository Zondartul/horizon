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

/*
void GUIscrollbarBar::render(){
	if(GUIoptions.push){pushRenderOptions();}
	vec2 pos = getMousePos();
	
	vec3 oldColor = bgColor;
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	if(GUIoptions.push){popRenderOptions();}
}
*/

void GUIscrollbarBar::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}

	vec2 pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			pressed = true;
			event.maskEvent();
			offset = area.start-pos;
			//scroll->saveOffsets();
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
	//btnDown->moveTo({0,20});
	addChild(btnDown);

	vbar = new GUIscrollbarBar();
	vbar->scroll = this;
	vbar->isClient = false;
	//vbar->setSize({20,30});
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
	constructed = true;
	invalidate();
}

/*
GUIbase* GUIscrollbar::addChild(GUIbase* child){
    bool oldSE = scrollingEnabled;
    if(scrollingEnabled){disableScrolling();}
    GUIframe::addChild(child);
	initialOffsets[child] = child->area.start;
    if(oldSE){enableScrolling();}
    return this;
}
*/

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
		//if(mouseover){
		//	pressed = true;
		//	event.maskEvent();
		//	offset = area.start-pos;
		//}
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
	//printf("======setScrollOffset %s\n",toString(newScrollOffset).c_str());
	invalidate();
	float x =  (float)newScrollOffset.x / (innerSize.x-clientArea.size.x);
	float y =  (float)newScrollOffset.y / (innerSize.y-clientArea.size.y);
	setScrollRatio({x,y});
	invalidate();
	//printf("======new offset %s\n",toString(scrollOffset()).c_str());
	return this;
}

// vec2 GUIscrollbar::scrollRatio(){
	// rect vtrack = vtrackRect();
	// rect htrack = htrackRect();
	// float xoffset = /*children[5]*/hbar->area.start.x - htrack.start.x;
	// float x = (float)xoffset / (htrack.size.x-/*children[5]*/hbar->area.size.x);
	// float yoffset = /*children[2]*/vbar->area.start.y - vtrack.start.y;
	// float y = (float)yoffset / (vtrack.size.y-/*children[2]*/vbar->area.size.y);
	// return {x,y};
// }
GUIscrollbar *GUIscrollbar::setScrollRatio(vec2 newScrollRatio){
	//printf("======setScrollRatio %s\n",toString(newScrollRatio).c_str());
	invalidate();
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = newScrollRatio.x * (htrack.size.x-/*children[5]*/hbar->area.size.x);
	/*children[5]*/hbar->area = /*children[5]*/hbar->area.moveTo({xoffset + htrack.start.x,/*children[5]*/hbar->area.start.y});
	float yoffset = newScrollRatio.y * (vtrack.size.y-/*children[2]*/vbar->area.size.y);
	/*children[2]*/vbar->area = /*children[2]*/vbar->area.moveTo({/*children[2]*/vbar->area.start.x,yoffset + vtrack.start.y});
	invalidate();
	//printf("======new ratio: %s\n",toString(scrollRatio()).c_str());
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
		/*children[0]*/btnUp->setHidden(!vertical);
		/*children[1]*/btnDown->setHidden(!vertical);
		/*children[2]*/vbar->setHidden(!vertical);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

GUIscrollbar *GUIscrollbar::setHorizontal(bool newHorizontal){
	vec2 offset = scrollOffset();
	horizontal = newHorizontal;
	if(children.size() >= 6){
		/*children[3]*/btnLeft->setHidden(!horizontal);
		/*children[4]*/btnRight->setHidden(!horizontal);
		/*children[5]*/hbar->setHidden(!horizontal);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

GUIscrollbar *GUIscrollbar::enableScrolling(){
    //if(scrollingEnabled){disableScrolling();}
    //for(int I = 6; I < children.size(); I++){
    //    GUIbase *child = children[I];
    //    initialOffsets[child] = child->area.start;
    //}
    scrollingEnabled = true;
    invalidate();
    return this;
}

GUIscrollbar *GUIscrollbar::disableScrolling(){
    //if(scrollingEnabled){
    //    for(int I = 6; I < children.size(); I++){
    //        GUIbase *child = children[I];
    //        if(initialOffsets.count(child)){
    //            child->area = child->area.moveTo(initialOffsets[child]);
    //        }
    //    }
    //}
    scrollingEnabled = false;
    invalidate();
    return this;
}

/*
void GUIscrollbar::saveOffsets(){
	for(auto I = children.begin(); I != children.end(); I++){
		GUIbase *child = *I;
		initialOffsets[child] = child->area.start;
	}
}
*/

void GUIscrollbar::renderLogic(){

	//vec2 offset = {0,0};
	//if(children.size() >= 6){
	//	offset = scrollOffset();
	//}
	for(int I = 0; I < children.size(); I++){
		if(!children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
			if(GUIoptions.debug){
				drawRectOutlineColored(children[I]->visibleArea(),{255,0,0});
			}else{
				setScissoring(true);
				//setScissor(children[I]->visibleArea());
				setScissor(visibleArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(GUIoptions.push){popRenderOptions();}
		}
	}

	//setColor({255,0,0});
	//drawRectOutline(visibleClientArea());
	//setScissoring(true);
	//setScissor(visibleClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
		//	children[I]->area=children[I]->area.moveBy(-offset);
			//setScissoring(true);
			//setScissor(children[I]->visibleArea());
            //drawRectOutline(children[I]->visibleArea());
			setScissoring(true);
			setScissor(visibleClientArea());
			
			children[I]->render();
			children[I]->renderLogic();
		//	children[I]->area=children[I]->area.moveBy(offset);
			if(GUIoptions.push){popRenderOptions();}
		}
	}
	//popRenderOptions();

}

/*
void GUIscrollbar::render(){
	if(GUIoptions.push){pushRenderOptions();}
	GUIframe::render();
	setAlpha(128);
	setColor(bgColor*0.9);
	
	if(vertical){
		rect R = vtrackRect().setStart(vtrackRect().start-(vec2){0,21})\
					.setEnd(vtrackRect().end+(vec2){0,21});
		drawRect(thisToWorld(R));
	}
	if(horizontal){
		rect R = htrackRect().setStart(htrackRect().start-(vec2){21,0})\
					.setEnd(htrackRect().end+(vec2){21,0});
		drawRect(thisToWorld(R));
	}
	if(GUIoptions.push){popRenderOptions();}
}
*/

void GUIscrollbar::invalidate(){
	//printf("scrollbar invalidate");
	if(parent && bSizeToParent){
		//printf("+parent\n");
		area = area.setStart(vec2(0,0)).setSize(parent->clientArea.size);
	}else{
		//printf(", no parent\n");
	}
	clientArea = clientArea.setStart({1,1}).setEnd(area.size-vec2{vertical? 22: 0, horizontal? 22:0});
	if(/*children.size() >= 6*/ constructed){
		rect vtrack = vtrackRect();
		rect htrack = htrackRect();

		//put the up-button in it's place
		/*children[0]*/btnUp->moveTo(vtrack.start-vec2{0,21});

		//put the down-button in it's place
		/*children[1]*/btnDown->moveTo(vtrack.end-/*children[1]*/btnDown->area.size+vec2{0,21});

		//put the vertical scrollbar bar in it's place
		rect Vr = /*children[2]*/vbar->area;
		Vr.size = {20,trackDimensions().y * areaRatio().y};
		Vr.size = clamp(Vr.size,{0,0},vtrack.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.end)-Vr.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.start));
		/*children[2]*/vbar->area = Vr;

		//put the left-button in it's place
		/*children[3]*/btnLeft->moveTo(htrack.start-vec2{21,0});

		//put the right-button in it's place
		/*children[4]*/btnRight->moveTo(htrack.end-/*children[4]*/btnRight->area.size+vec2{21,0});

		//put the horizontal scrollbar bar in it's place
		rect Hr = /*children[5]*/hbar->area;
		Hr.size = {trackDimensions().x * areaRatio().x,20};
		Hr.size = clamp(Hr.size,{0,0},htrack.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.end)-Hr.size);
		Hr = Hr.moveTo(htrack.clamp(Hr.start));
		/*children[5]*/hbar->area = Hr;

		// printf("--------------\n");
		// printf("vtrackRect: %s\n",toString(vtrackRect()).c_str());
		// printf("htrackRect: %s\n",toString(htrackRect()).c_str());
		// printf("trackDimensions: %s\n",toString(trackDimensions()).c_str());
		 //printf("areaRatio: %s\n",toString(areaRatio()).c_str());
		 //printf("normalizedAreaRatio: %s\n",toString(normalizedAreaRatio()).c_str());
		 //printf("scrollRatio: %s\n",toString(scrollRatio()).c_str());
		// printf("scrollOffset: %s\n",toString(scrollOffset()).c_str());

	}
	//displace children by offset
	/* //old (uses children on scrollbar)
	if(children.size() >= 6){ //aka if all is constructed
        if(scrollingEnabled){
            offset = scrollOffset();
            //printf("offset = %s, prevoffset = %s\n",toCString(offset),toCString(prevoffset));
            vec2 dv = offset-prevoffset;
            //printf("--------------\n");
            for(int I = 6; I < children.size(); I++){
                //if(children[I]->isClient && !children[I]->hidden){
                    //children[I]->area=children[I]->area.moveBy(-dv);
                    GUIbase *child = children[I];
                    if(!initialOffsets.count(child)){initialOffsets[child] = child->area.start;}
                    //vec2 vinit = initialOffsets[child];
                    //vec2 vfin = vinit + offset;
                    //printf("ch[%d], vini = %s, vfin = %s\n",I,toCString(vinit),toCString(vfin));
                    child->area = child->area.moveTo(initialOffsets[child]-offset);
                //}
            }
            //printf("---------------\n");
            prevoffset = offset;
        }
	}
	*/
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
/*
//nobody uses this?
vec2 GUIscrollbar::normalizedAreaRatio(){
	vec2 ar = areaRatio();
	ar.x = min(ar.x,1.0);
	ar.y = min(ar.y,1.0);
	return ar;
}
*/

vec2 GUIscrollbar::scrollRatio(){
	rect vtrack = vtrackRect();
	rect htrack = htrackRect();
	float xoffset = /*children[5]*/hbar->area.start.x - htrack.start.x;
	float x = (float)xoffset / (htrack.size.x-/*children[5]*/hbar->area.size.x);
	float yoffset = /*children[2]*/vbar->area.start.y - vtrack.start.y;
	float y = (float)yoffset / (vtrack.size.y-/*children[2]*/vbar->area.size.y);
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
	//printf("%s::setProperty(%s)=[%s]\n",getType().c_str(),key.c_str(),val.c_str());

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
