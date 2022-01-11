#include "gui/Gui5/behaviors/GUI5behaviorHighlight.h"
#include "gui/Gui5/GUI5Base.h"
#include "input/inputMessage.h"

GUI5behaviorHighlight::GUI5behaviorHighlight(){
	mouseover = false;
	newColors["border"] = (color4i){255,255,255,255};
	newColors["text"] = (color4i){255,255,255,255};
}

void GUI5behaviorHighlight::run(GUI5base *B, message *msg){
	if(msg->type == "mouse_move"){
		vec2i newPos = ((message_mouse_move*)msg)->newPos;
		if((B->hasExclusiveMouseover(newPos)) != mouseover){
			mouseover = !mouseover;
			for(map<string, color4i>::iterator I = newColors.begin(); I != newColors.end(); I++){
				color4i oldColor = B->colors[I->first];
				B->colors[I->first] = I->second;
				I->second = oldColor;
			}
		}
	}
}