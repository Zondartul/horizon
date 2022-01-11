#include "GUIbase.h"

//GUIbase
GUIbase::GUIbase(){
	parent = NULL;
	isClient = true;
	hidden = false;
	suppressInvalidate = false;
	area = defaultarea;
	clientArea = defaultclientarea;
}

GUIbase::~GUIbase(){
	if(parent){parent->removeChild(this);}
}

GUIbase *GUIbase::addChild(GUIbase *child){
	child->parent = this;
	children.push_back(child); //somehow, adding them to the front screws everything up
	channel.addListenerFront(child);
	invalidateTree();
	//invalidate();
	//child->invalidate();
	return this;
}

void GUIbase_debugChildren(GUIbase *b){
	if(!b){error("'this' is null");}
	printf("printing %p's %d children:\n",b,b->children.size());
	if(!b->children.size()){printf("%p has no children\n");return;}
	int i = 0;
	for(auto I = b->children.begin(); I != b->children.end(); I++){
		GUIbase *b2 = *I;
		string type = typeid(*b2).name();
		printf("%d: %p (%s)\n",i,*I,type.c_str());
		i++;
	}
}

GUIbase *GUIbase::removeChild(GUIbase *child){
	//printf("removeChild called\n");
	//GUIbase_debugChildren(this);
	bool found = false;
	int i = 0;
	for(auto I = children.begin(); I != children.end();){ //no I++ here
		if(*I == child){
			//printf("erased %p, %d\n",*I,i++);
			channel.removeListener(child);
			I = children.erase(I);
			found = true;
		}else{
			I++;
		}
	}
	if(!found){printf("couldn't erase %p: not found\n",child);}
	invalidateTree();
	//printf("invalidated\n");
	return this;
}

GUIbase *GUIbase::setSize(vec2 newSize){
	area = area.setSize(newSize);
	invalidate();
	return this;
}

GUIbase *GUIbase::moveTo(vec2 newstart){
	area = area.moveTo(newstart);
	//printf("moveTo: new area = %s\n",toString(area).c_str());
	invalidate();
	return this;
}

GUIbase *GUIbase::setClientArea(rect newclientarea){
	clientArea = newclientarea;
	return this;
}

GUIbase *GUIbase::setHidden(bool newHidden){
	hidden = newHidden;
	return this;
}

GUIbase *GUIbase::sizeToContents(){
	//printf("sizeToContents()\n");
	//printf("stc area1: %s\n",toString(area).c_str());
	//printf("stc cli1:  %s\n",toString(clientArea).c_str());
	float maxx = 0;
	float maxy = 0;
	for(int I = 0; I < children.size(); I++){
		//printf("ch 1: %s\n",toString(children[I]->area).c_str());
		maxx = max(maxx, children[I]->area.end.x);
		maxy = max(maxy, children[I]->area.end.y);
		//printf("maxx = %f\n",maxx);
		//printf("maxy = %f\n",maxy);
	}
	vec2 newsize = {maxx,maxy};
	vec2 diff = newsize - clientArea.size;
	clientArea = clientArea.setSize(newsize);
	area = area.setSize(area.size+diff);
	//printf("stc area2: %s\n",toString(area).c_str());
	//printf("stc cli2:  %s\n",toString(clientArea).c_str());
	return this;
}

void GUIbase::renderLogic(){
	if(!parent){GUIsetFavoriteRenderOptions();}
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
	//cropScissor(worldClientArea());
	for(int I = 0; I < children.size(); I++){
		if(children[I]->isClient && !children[I]->hidden){
			if(GUIoptions.push){pushRenderOptions();}
			if(GUIoptions.debug){
				drawRectOutlineColored(children[I]->visibleArea(),{128,0,0});
			}else{
			setScissoring(true);
			setScissor(children[I]->visibleArea());
			}
			children[I]->render();
			children[I]->renderLogic();
			if(GUIoptions.push){popRenderOptions();}
		}
	}
	//popRenderOptions();
}

void GUIbase::render(){}

void GUIbase::onEvent(eventKind event){
	// for(int I = 0; I < children.size(); I++){
		// if(!children[I]->hidden){
			// children[I]->onEvent(event);
		// }
	// }
	if(event.type == EVENT_MOUSE_MOVE){
		recalcMouseover();	//this check needs to be already
							//done by the time children start handling events
	}
	event.maskEvent(channel.publishEventParallelMaskable(event));
}

GUIbase *GUIbase::root(){
	if(parent){
		GUIbase *p = parent;
		while(p->parent){p = p->parent;}
		return p;
	}else{
		return this;
	}
}

void GUIbase::invalidate(){
	//printf("invalidate %p (%s)\n",this,typeid(this).name());
	clientArea = rect().setStart({1,1}).setEnd(area.size-(vec2){1,1});//rect(newarea.size);
}

void GUIbase::invalidateTree(){
	if(suppressInvalidate){return;}
	//set invalidateSupress and invalidate children (inv downwards)
	//btw, when an external change is done to an element, it probably supresses invalidations and invalidates upwards and downwards...
	//or invalidates from top.
	if(parent){
		root()->invalidateTree();
	}else{
		invalidateDown();
	}
}

void GUIbase::invalidateDown(){
	invalidate();
	bool prevsuppress = suppressInvalidate;
	suppressInvalidate = true;
	for(auto I = children.begin(); I != children.end(); I++){
		(*I)->invalidateDown();
	}
	suppressInvalidate = prevsuppress;
}

//in the future, this could be used to classify mouse
//as "over border/title/other parts"
void GUIbase::recalcMouseover(){
	vec2 pos = getMousePos();
	//not mouseover if mouse is somewhere else entirely
	if(!visibleArea().contains(pos)){mouseover = false; return;}
	if(parent){
		//not mouseover if parent doesn't have mouseover
		if(!parent->mouseover){mouseover = false; return;}
		//not mouseover if any of higher siblings have mouseover
		auto I = parent->children.begin();
		while(*I != this){I++;}
		I++;
		for(;I != parent->children.end(); I++){
			if((*I)->mouseover){mouseover = false; return;}
		}
	}
	mouseover = true;
}

//if something has coordinates L within
//the Area of this element,
//it was coordinates W in the world.
vec2 GUIbase::thisToWorld(vec2 L){
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
vec2 GUIbase::clientToWorld(vec2 L){
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
		vec2 scr = getScreenSize();
		return rect(scr);
	}
}
rect GUIbase::worldClientArea(){
	if(parent){
		return thisToWorld(clientArea);
	}else{
		vec2 scr = getScreenSize();
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

