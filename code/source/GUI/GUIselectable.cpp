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
	//GUIbase::onEvent(event);
	if(event.type == EVENT_MOUSE_MOVE){
		recalcMouseover();	//this check needs to be already
							//done by the time children start handling events
	}
	//we can make the children clickable by uncommenting this line
	//event.maskEvent(channel.publishEventParallelMaskable(event));
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
	selected = true;
	
	bool shift = keyboardState["Left Shift"];
	bool ctrl = keyboardState["Left Ctrl"];
	printf("shift = %d, ctrl = %d\n",shift,ctrl);
	if(parent){
		//1. selecting this item should deselect other items
		GUIselectionGroup *SG = dynamic_cast<GUIselectionGroup*>(parent);
		if(SG && !ctrl){
			if(shift){
				int firstSelected = -1;
				int lastSelected = -1;
				for(int I = 0; I < SG->children.size(); I++){
					GUIselectable *elSel = dynamic_cast<GUIselectable*>(SG->children[I]);
					if(elSel && elSel->selected && (firstSelected < 0)){firstSelected = I;} 
					if(elSel && elSel->selected && (firstSelected >= 0)){lastSelected = I;}
				}
				for(int I = 0; I < SG->children.size(); I++){
					GUIselectable *elSel = dynamic_cast<GUIselectable*>(SG->children[I]);
					if((I >= firstSelected) && (I <= lastSelected)){
						elSel->selected = true;
					}else{
						elSel->selected = false;
					}
				}
			}else{
				for(int I = 0; I < SG->children.size(); I++){
					GUIselectable *elSel = dynamic_cast<GUIselectable*>(SG->children[I]);
					if(elSel && (elSel != this)){elSel->selected = false;}
				}
			}
			// bool hasOtherSels = false;
			// for(auto I = SG->children.begin(); I != SG->children.end(); I++){
				// GUIselectable *elSel = dynamic_cast<GUIselectable*>(*I);
				// if(elSel && elSel != this && elSel->selected){hasOtherSels = true; break;}
			// }
			
			// bool rangeStarted = false;
			// bool rangeEnded = false;
			// int i = 0;
			// for(auto I = SG->children.begin(); I != SG->children.end(); I++){
				// GUIbase *el = *I;
				// GUIselectable *elSel = dynamic_cast<GUIselectable*>(el);
				// i++;
				// printf("[%d](%d,%d):",i,(elSel == this),(elSel->selected));
				// if(elSel){
					// printf("-1");
					// if(shift){
						// printf("-1");
						// if(!rangeStarted){
							// printf("-a");
							// if(elSel != this){
								// printf("-1");
								// if(elSel->selected){printf("-1");rangeStarted = true;}
							// }else{printf("-2"); rangeStarted = true; if(!hasOtherSels){rangeEnded = true;}}
						// }else if(rangeStarted && !rangeEnded){
							// printf("-b");
							// elSel->selected = true;
							// if(elSel == this){printf("-1");rangeEnded = true;}
						// }else if(rangeEnded){
							// printf("-c");
							// elSel->selected = false;
						// }
					// }else{
						// printf("-2");
						// if(elSel != this){printf("-1"); elSel->selected = false;}
					// }
				// }
				// printf("\n");
			// }
		}
		//2. selecting this item should put it on top of other items
		// (this breaks shift-clicking)
		//parent->moveChildToTheTop(this);
	}
}

string GUIselectable::getType(){return "GUIselectable";}











