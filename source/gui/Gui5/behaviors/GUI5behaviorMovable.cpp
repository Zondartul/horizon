#include "gui/Gui5/behaviors/GUI5behaviorMovable.h"
#include "gui/Gui5/GUI5base.h"
#include "input/inputMessage.h"

GUI5behaviorMovable::GUI5behaviorMovable(){movable = true; moving = false; moveOffset = (vec2i){0,0};}
GUI5behaviorMovable &GUI5behaviorMovable::setMovable(bool movable){this->movable = movable; return *this;}
void GUI5behaviorMovable::run(GUI5base *B, message *msg){
	if(msg->type == "mouse_move"){
		if(moving){
			vec2i deltaPos = ((message_mouse_move*)msg)->deltaPos;
			vec2i newPos = ((message_mouse_move*)msg)->newPos;
			if(B->parent){newPos = B->area.getParent()->toWorld().clamp(newPos);}
			B->area.moveStart(moveOffset + newPos);
			B->invalidate();
		}
	}
	if(msg->type == "lmb_down"){
		vec2i pos = ((message_mouse*)msg)->newPos;
		if(movable && B->hasAnyMouseover(pos)){
			moving = true;
			moveOffset = B->area.getStart() - pos;
			msg->handled = true;
		}
	}
	if(msg->type == "lmb_up"){
		moving = false;
	}
}