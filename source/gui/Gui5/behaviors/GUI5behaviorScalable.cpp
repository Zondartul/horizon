#include "gui/Gui5/behaviors/GUI5behaviorScalable.h"
#include "input/inputMessage.h"
#include "gui/Gui5/GUI5base.h"
#include "resource/cursor.h"

GUI5behaviorScalable::GUI5behaviorScalable(){
	resizable = true;
	resizeState = 0;
	borderSize = 3;
	cursorSet = false;
}
GUI5behaviorScalable &GUI5behaviorScalable::setResizable(bool resizable){
	this->resizable = resizable;
	return *this;
}
GUI5behaviorScalable &GUI5behaviorScalable::setBorderSize(int size){
	borderSize = size;
	return *this;
}
void GUI5behaviorScalable::run(GUI5base *B, message *msg){
	if(msg->type == "mouse_move"){
		vec2i newPos = ((message_mouse_move*)msg)->newPos;
		if(resizeState){
			if(B->parent){newPos = B->area.getParent()->toWorld().clamp(newPos);}
			switch(resizeState){
				case 1:	//upper
					B->area.setStart({B->area.getx(),min(B->area.gety()+newPos.y-B->area.toWorld().gety(), B->area.gety2()-3*borderSize)});
				break;
				case 2:	//lower
					B->area.setEnd({B->area.getx2(),max(B->area.gety2()+newPos.y-B->area.toWorld().gety2(), B->area.gety()+3*borderSize)});
				break;
				case 3:	//right
					B->area.setStart({min(B->area.getx()+newPos.x-B->area.toWorld().getx(), B->area.getx2()-3*borderSize), B->area.gety()});
				break;
				case 4:	//left
					B->area.setEnd({max(B->area.getx2()+newPos.x-B->area.toWorld().getx2(), B->area.getx()+3*borderSize), B->area.gety2()});
				break; //apologies for the word salad
			}
			B->invalidate();
		}
		//mouseover cursors
		if(B->hasAnyMouseover(newPos)){
			if(resizable){
				vector<rect> borders = B->area.toWorld().getBorders(borderSize);
				if(borders[0].contains(newPos)){setCursor(getCursorHandle("sizens")); cursorSet = true;}
				else if(borders[1].contains(newPos)){setCursor(getCursorHandle("sizens")); cursorSet = true;}
				else if(borders[2].contains(newPos)){setCursor(getCursorHandle("sizewe")); cursorSet = true;}
				else if(borders[3].contains(newPos)){setCursor(getCursorHandle("sizewe")); cursorSet = true;}
				else{setCursor(getCursorHandle("arrow"));}
			}
		}else{
			if(cursorSet){setCursor(getCursorHandle("arrow")); cursorSet = false;}
		}
	}
	if(msg->type == "lmb_down"){
		vec2i pos = ((message_mouse*)msg)->newPos;
		if(resizable && B->hasAnyMouseover(pos)){
			resizeState = 0;
			vector<rect> borders = B->area.toWorld().getBorders(borderSize);
			if(borders[0].contains(pos)){printf("upper border!\n"); resizeState = 1;}
			if(borders[1].contains(pos)){printf("lower border!\n"); resizeState = 2;}
			if(borders[2].contains(pos)){printf("left border!\n"); resizeState = 3;}
			if(borders[3].contains(pos)){printf("right border!\n"); resizeState = 4;}
			if(resizeState){msg->handled = true;}
		}
	}
	if(msg->type == "lmb_up"){
		resizeState = 0;
	}
}