#include "GUI_internal.h"
string GUIselectionGroup::getType(){return "GUIselectionGroup";}
void GUIselectionGroup::onEvent(eventKind event){
	GUIbase::onEvent(event);
	if(event.isMasked()){return;}
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		for(auto I = children.begin(); I != children.end(); I++){
			GUIselectable *elSel = dynamic_cast<GUIselectable*>(*I);
			if(elSel){elSel->selected = false;}
		}
	}
}
GUIselectable::GUIselectable(){
	selected = false;
	group = nullptr;
	pressed = false;
}
void GUIselectable::onEvent(eventKind event){
	if(event.type == EVENT_MOUSE_MOVE){
		recalcMouseover();	
	}
	if(event.isMasked()){return;}
	vec2 pos = getMousePos();
	if(event.type == EVENT_MOUSE_BUTTON_DOWN){
		if(mouseover){
			printf("click selectable\n");
			event.maskEvent();
			pressed = true;	
			offset = area.start-pos;			
			select();
		}
	}
	if(event.type == EVENT_MOUSE_BUTTON_UP){
		if(pressed){
			pressed = false;
			printf("unclick selectable\n");
			event.maskEvent();
		}
	}
	if(event.type == EVENT_MOUSE_MOVE){
		if(moveable && pressed){moveTo(pos+offset);} 
	}
}
void GUIselectable::select(){
	auto& keyboardState = Gb->gs_window->g_keyboardState;
	selected = true;
	bool shift = keyboardState["Left Shift"];
	bool ctrl = keyboardState["Left Ctrl"];
	printf("shift = %d, ctrl = %d\n",shift,ctrl);
	if(parent){
		GUIselectionGroup *SG = dynamic_cast<GUIselectionGroup*>(parent);
		if(SG && !ctrl){
			if(shift){
				int firstSelected = -1;
				int lastSelected = -1;
				for(int I = 0; I < (int)SG->children.size(); I++){
					GUIselectable *elSel = dynamic_cast<GUIselectable*>(SG->children[I]);
					if(elSel && elSel->selected && (firstSelected < 0)){firstSelected = I;} 
					if(elSel && elSel->selected && (firstSelected >= 0)){lastSelected = I;}
				}
				for(int I = 0; I < (int)SG->children.size(); I++){
					GUIselectable *elSel = dynamic_cast<GUIselectable*>(SG->children[I]);
					if((I >= firstSelected) && (I <= lastSelected)){
						elSel->selected = true;
					}else{
						elSel->selected = false;
					}
				}
			}else{
				for(unsigned int I = 0; I < SG->children.size(); I++){
					GUIselectable *elSel = dynamic_cast<GUIselectable*>(SG->children[I]);
					if(elSel && (elSel != this)){elSel->selected = false;}
				}
			}
		}
	}
}
string GUIselectable::getType(){return "GUIselectable";}
