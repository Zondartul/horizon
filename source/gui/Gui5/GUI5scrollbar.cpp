#include "gui/Gui5/GUI5scrollbar.h"

GUI5scrollbar::GUI5scrollbar(){
	vertical = true;
	horizontal = true;
	min = (vec2i){0,0};
	max = (vec2i){1,1};
	cur = (vec2i){0,0};
}
GUI5scrollbar &GUI5scrollbar::setVertical(bool v){vertical = v; return *this;}
GUI5scrollbar &GUI5scrollbar::setHorizontal(bool h){horizontal = h; return *this;}
GUI5scrollbar &GUI5scrollbar::setVerticalLimits(int start, int end){min.y = start; max.y = end; return *this;}
GUI5scrollbar &GUI5scrollbar::setHorizontalLimits(int start, int end){min.x = start; max.x = end; return *this;}
GUI5scrollbar &GUI5scrollbar::setVerticalPos(int pos){cur.y = pos; return *this;}
GUI5scrollbar &GUI5scrollbar::setHorizontalPos(int pos){cur.x = pos; return *this;}

// RENDER DIAGRAM
//
//					a1--b1
//					| e1|
//					c1--d1
//					|   |
//					|	|
//					a2--b2
//					|///|
//					|///|
//					|///|
//					|///|
//					c2--d2
//					|   |
//					|	|
//					a1--b1
//					| e1|
//					c1--d1
//
//
//
//


void GUI5scrollbar::render(){
	
}
void GUI5scrollbar::layout(){}
void GUI5scrollbar::receiveMessageExtra(message *msg){}