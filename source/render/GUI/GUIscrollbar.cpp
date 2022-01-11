#include "GUIscrollbar.h"

//GUIscrollbarBar
GUIscrollbarBar::GUIscrollbarBar(){
	mouseover = false;
	pressed = false;
	offset = {0,0};
	hoverColor = bgColor*0.9;
	pressedColor = bgColor*0.8;
}

void GUIscrollbarBar::render(){
	if(GUIoptions.push){pushRenderOptions();}
	vec2 pos = getMousePos();
	//vec2 min = {parent->area.size.x-21,21};
	//vec2 max = {parent->area.size.x-21,parent->area.size.y-21-area.size.y};
	//if(pressed){moveTo(clamp(offset+pos, min,max));if(parent){parent->invalidate();}}
	//else{moveTo(clamp(area.start, min, max));}

	//mouseover = visibleArea().contains(pos);
	//mouseover = thisToWorld(rect(area.size)).contains(pos);
	vec3 oldColor = bgColor;
	if(mouseover){setAlpha(196); bgColor = hoverColor;}
	if(pressed){setAlpha(255); bgColor = pressedColor;}
	GUIframe::render();
	bgColor = oldColor;
	if(GUIoptions.push){popRenderOptions();}
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

//===========================================================================================

//GUIscrollbar
GUIscrollbar::GUIscrollbar(){
	innerSize = area.size;
	//scrollPos = {0,0};
	bsizeToParent = false;
	vertical = true;
	horizontal = true;
	offset = vec2(0,0);
	prevoffset = vec2(0,0);
    scrollingEnabled = true;

	GUIbutton *btnUp = new GUIbutton();
	btnUp->isClient = false;
	btnUp->setText("^");
	btnUp->setSize({20,20});
	addChild(btnUp);

	GUIbutton *btnDown = new GUIbutton();
	btnDown->isClient = false;
	btnDown->setText("v");
	btnDown->setSize({20,20});
	//btnDown->moveTo({0,20});
	addChild(btnDown);

	GUIscrollbarBar *vbar = new GUIscrollbarBar();
	vbar->isClient = false;
	//vbar->setSize({20,30});
	addChild(vbar);

	GUIbutton *btnLeft = new GUIbutton();
	btnLeft->isClient = false;
	btnLeft->setText("<");
	btnLeft->setSize({20,20});
	addChild(btnLeft);

	GUIbutton *btnRight = new GUIbutton();
	btnRight->isClient = false;
	btnRight->setText(">");
	btnRight->setSize({20,20});
	addChild(btnRight);

	GUIscrollbarBar *hbar = new GUIscrollbarBar();
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
	invalidate();
}

GUIbase* GUIscrollbar::addChild(GUIbase* child){
    bool oldSE = scrollingEnabled;
    if(scrollingEnabled){disableScrolling();}
    GUIframe::addChild(child);
    if(oldSE){enableScrolling();}
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

GUIscrollbar *GUIscrollbar::setInnerSize(vec2 newInnerSize){
	innerSize = newInnerSize;
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
	// float xoffset = children[5]->area.start.x - htrack.start.x;
	// float x = (float)xoffset / (htrack.size.x-children[5]->area.size.x);
	// float yoffset = children[2]->area.start.y - vtrack.start.y;
	// float y = (float)yoffset / (vtrack.size.y-children[2]->area.size.y);
	// return {x,y};
// }
GUIscrollbar *GUIscrollbar::setScrollRatio(vec2 newScrollRatio){
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
	return this;
}

GUIscrollbar *GUIscrollbar::sizeToParent(bool b){
	bsizeToParent = b;
	return this;
}

GUIscrollbar *GUIscrollbar::setVertical(bool newVertical){
	vec2 offset = scrollOffset();
	vertical = newVertical;
	if(children.size() >= 6){
		children[0]->setHidden(!vertical);
		children[1]->setHidden(!vertical);
		children[2]->setHidden(!vertical);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

GUIscrollbar *GUIscrollbar::setHorizontal(bool newHorizontal){
	vec2 offset = scrollOffset();
	horizontal = newHorizontal;
	if(children.size() >= 6){
		children[3]->setHidden(!horizontal);
		children[4]->setHidden(!horizontal);
		children[5]->setHidden(!horizontal);
	}
	invalidate();
	setScrollOffset(offset);
	return this;
}

GUIscrollbar *GUIscrollbar::enableScrolling(){
    if(scrollingEnabled){disableScrolling();}
    for(int I = 6; I < children.size(); I++){
        GUIbase *child = children[I];
        initialOffsets[child] = child->area.start;
    }
    scrollingEnabled = true;
    invalidate();
    return this;
}

GUIscrollbar *GUIscrollbar::disableScrolling(){
    if(scrollingEnabled){
        for(int I = 6; I < children.size(); I++){
            GUIbase *child = children[I];
            if(initialOffsets.count(child)){
                child->area = child->area.moveTo(initialOffsets[child]);
            }
        }
    }
    scrollingEnabled = false;
    invalidate();
    return this;
}

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
				setScissor(children[I]->visibleArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(GUIoptions.push){popRenderOptions();}
		}
	}

	//setColor({255,0,0});
	//drawRectOutline(visibleClientArea());
	setScissoring(true);
	setScissor(visibleClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
		//	children[I]->area=children[I]->area.moveBy(-offset);
			//setScissoring(true);
			//setScissor(children[I]->visibleArea());
            //drawRectOutline(children[I]->visibleArea());
			children[I]->render();
			children[I]->renderLogic();
		//	children[I]->area=children[I]->area.moveBy(offset);
			if(GUIoptions.push){popRenderOptions();}
		}
	}
	//popRenderOptions();

}

void GUIscrollbar::render(){
	if(GUIoptions.push){pushRenderOptions();}
	GUIframe::render();
	setAlpha(128);
	setColor(bgColor*0.9);
	//rect R = rect().setStart({area.size.x-22,1}).setEnd({area.size.x-1,area.size.y-1});
	//drawRect(thisToWorld(R));
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

void GUIscrollbar::invalidate(){
	//printf("scrollbar invalidate");
	if(parent && bsizeToParent){
		//printf("+parent\n");
		area = area.setSize(parent->clientArea.size);
	}else{
		//printf(", no parent\n");
	}
	clientArea = clientArea.setStart({1,1}).setEnd(area.size-(vec2){vertical? 22: 0, horizontal? 22:0});
	if(children.size() >= 6){
		rect vtrack = vtrackRect();
		rect htrack = htrackRect();

		//put the up-button in it's place
		children[0]->moveTo(vtrack.start-(vec2){0,21});

		//put the down-button in it's place
		children[1]->moveTo(vtrack.end-children[1]->area.size+(vec2){0,21});

		//put the vertical scrollbar bar in it's place
		rect Vr = children[2]->area;
		Vr.size = {20,trackDimensions().y * areaRatio().y};
		Vr.size = clamp(Vr.size,{0,0},vtrack.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.end)-Vr.size);
		Vr = Vr.moveTo(vtrack.clamp(Vr.start));
		children[2]->area = Vr;

		//put the left-button in it's place
		children[3]->moveTo(htrack.start-(vec2){21,0});

		//put the right-button in it's place
		children[4]->moveTo(htrack.end-children[4]->area.size+(vec2){21,0});

		//put the horizontal scrollbar bar in it's place
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
		 //printf("areaRatio: %s\n",toString(areaRatio()).c_str());
		 //printf("normalizedAreaRatio: %s\n",toString(normalizedAreaRatio()).c_str());
		 //printf("scrollRatio: %s\n",toString(scrollRatio()).c_str());
		// printf("scrollOffset: %s\n",toString(scrollOffset()).c_str());

	}
	//displace children by offset
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
	float xoffset = children[5]->area.start.x - htrack.start.x;
	float x = (float)xoffset / (htrack.size.x-children[5]->area.size.x);
	float yoffset = children[2]->area.start.y - vtrack.start.y;
	float y = (float)yoffset / (vtrack.size.y-children[2]->area.size.y);
	return {x,y};
}
//float -> integer round down causes x,y to drift towards 0
vec2 GUIscrollbar::scrollOffset(){
	int x = (innerSize.x-clientArea.size.x) * scrollRatio().x;
	int y = (innerSize.y-clientArea.size.y) * scrollRatio().y;
	return clamp({x,y},{0,0},innerSize-clientArea.size);
}

