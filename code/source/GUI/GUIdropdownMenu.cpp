#include "GUI_internal.h"

GUIdropdownMenu::GUIdropdownMenu(){
	auto& globalChannel = G->gs_event->g_globalChannel;
	setSize(vec2(10,10));
	grid = new GUIgrid();
	grid->setSize(vec2(10,10));
	this->addChild(grid);
	noframe = true;
	constructed = true;
	globalChannel->addListener(this); 
}

GUIbutton *GUIdropdownMenu::addItem(string text){
	GUIbutton *btn = new GUIbutton();
	btn->setText(text);
	btn->setSize(vec2(80,24));
	btn->moveTo(vec2(0,0));
	
	grid->addChild(btn);
	grid->grid(btn);
	invalidate();
	return btn;
}

GUIbutton *GUIdropdownMenu::addItem(string text, function<void()> func){
	GUIbutton *btn = addItem(text);
	btn->F = func;
	invalidate();
	return btn;
}

GUIdropdownMenu *GUIdropdownMenu::addSubmenu(string text){
	
	
	
	
	
	GUIdropdownMenu *ddm = new GUIdropdownMenu();
	ddm->name = name+"_sub";
	submenus.push_back(ddm);
	ddm->isClient = false;
	ddm->hidden = true;
	
	GUIbutton *btn = addItem(text+" >",[=](){
		ddm->hidden = false;
	});
	ddm->btnMenu = btn;
	ddm->moveTo(vec2(btn->area.end.x,btn->area.start.y));
	addChild(ddm);
	invalidate();
	return ddm;
}

void GUIdropdownMenu::invalidate(){
	if(!constructed){return;}
	vec2 max_corner = grid->area.end;
	for(auto I = submenus.begin(); I != submenus.end(); I++){
		(*I)->invalidate();
		vec2 corner = (*I)->area.start+(*I)->grid->area.end; 
		max_corner = max(max_corner, corner);
	}
	area = area.setSize(max_corner);
	clientArea = area.setStart(vec2(0,0));;
}

float rectDist(rect R, vec2 pos){
	vec2 A = R.start;
	vec2 D = R.end;
	vec2 B(D.x,A.y);
	vec2 C(A.x,D.y);
	float dist = length(pos-A);
	dist = min(dist,length(pos-B));
	dist = min(dist,length(pos-C));
	dist = min(dist,length(pos-D));
	if(R.contains(pos)){dist = 0;}
	return dist;
}

float GUIdropdownMenu::getMouseDist(){
	vec2 pos = getMousePos();
	rect R = grid->worldArea();
	float dist = rectDist(R,pos);
	if(btnMenu){
		R = btnMenu->worldArea();
		dist = min(dist, rectDist(R,pos));
	}
	for(auto I = submenus.begin(); I != submenus.end(); I++){
		GUIdropdownMenu *ddm = *I;
		dist = min(dist, ddm->getMouseDist());
	}
	return dist;
}



void GUIdropdownMenu::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}

	if(event.type == EVENT_FRAME){
		
		if(!hidden){
			float dist = getMouseDist();
			if(dist == 0){hideCounter = 0;}
			else{hideCounter += (int)dist;}
			if(hideCounter > 100*10){
				hideCounter = 0;
				hidden = true;
			}
		}
	}
}

string GUIdropdownMenu::getType(){return "GUIdropdownMenu";}





