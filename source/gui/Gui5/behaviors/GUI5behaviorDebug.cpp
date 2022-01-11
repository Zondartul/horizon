#include "gui/Gui5/behaviors/GUI5behaviorDebug.h"
#include "gui/Gui5/GUI5base.h"
#include "input/inputMessage.h"

void GUI5behaviorDebug::run(GUI5base *B, message *msg){
	if(msg->type == "rmb_down"){
		//check mouseover
		vec2i newPos = ((message_mouse_move*)msg)->newPos;
		if(B->hasAnyMouseover(newPos)){	//faster than ExclusiveMouseover, exclusivity hopefully granted by event ordering
			B->debugmode = !B->debugmode;
			msg->handled = true;
		}
	}
}