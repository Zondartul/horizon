#include "util/rect.h"
#include "util/globals.h"
#include "assert.h"
rect::rect(){
	start = {0,0};
	end = {0,0};
	size = {0,0};
	parent = 0;
}
rect::rect(vec2i start, vec2i end){
	this->start = start;
	this->end = end;
	size = end-start;
	parent = 0;
}
rect &rect::setStart(vec2i A){setx(A.x);sety(A.y); return *this;}
rect &rect::setEnd(vec2i A){setx2(A.x);sety2(A.y); return *this;}
rect &rect::setSize(vec2i A){setw(A.x);seth(A.y); return *this;}
rect &rect::moveBy(vec2i A){setStart(start+A); setEnd(end+A); return *this;}
rect &rect::moveStart(vec2i A){vec2i size = getSize(); setStart(A); setSize(size); return *this;}
rect &rect::moveEnd(vec2i A){vec2i size = getSize(); setStart(A-size); setSize(size); return *this;}
rect &rect::setx(int x){start.x = x; size.x = end.x - start.x; return *this;} //changing start keeps end but changes size (resize)
rect &rect::sety(int y){start.y = y; size.y = end.y - start.y; return *this;}
rect &rect::setw(int w){size.x = w; end.x = start.x+size.x; return *this;}	//changing size keeps start but changes end (resize)
rect &rect::seth(int h){size.y = h; end.y = start.y+size.y; return *this;}
rect &rect::setx2(int x2){end.x = x2; size.x = end.x-start.x; return *this;}//changing end keeps start but changes size (resize)
rect &rect::sety2(int y2){end.y = y2; size.y = end.y-start.y; return *this;}
rect &rect::setParent(rect *parent){this->parent = parent; return *this;}
bool rect::contains(vec2i A){return (A.x >= start.x) && (A.x <= end.x) && (A.y >= start.y) && (A.y <= end.y);}
bool rect::contains(vec4i A){return contains((vec2i){A.x1,A.y1}) && contains((vec2i){A.x2,A.y2});}
bool rect::contains(rect A){return contains(A.start) && contains(A.end);}
vec2i rect::clamp(vec2i A){return {(int)::clamp(A.x, start.x, end.x),(int)::clamp(A.y,start.y,end.y)};} //nothing::something means "find something in global scope"
rect rect::clamp(rect A){
	return A.setStart(clamp(A.start)).setEnd(clamp(A.end));
}
rect rect::insert(rect A){
	vec2i diff = {0,0};
	if(A.start.x < start.x){diff.x = start.x - A.start.x;}
	else if(end.x < A.end.x){diff.x = end.x - A.end.x;}
	if(A.start.y < start.y){diff.y = start.y - A.start.y;}
	else if(end.y < A.end.y){diff.y = end.y - A.end.y;}
	return A.moveBy(diff);
}
rect rect::toParent(rect A){	//A is in local, need it in parent's coords.
	return A.setStart(toParent(A.start)).setEnd(toParent(A.end));
}
rect rect::toParent(){			//need this in parent's coords.
	//rect A;
	//A.setStart({0,0}).setSize(size);
	//return toParent(A);
	return *this; //... it's already in parent's coords.
}
rect rect::toWorld(rect A){		//A is in local, need it in world coords.
	A.setStart(toWorld(A.start)).setEnd(toWorld(A.end));
	return A;
}
rect rect::toWorld(){			//need this rect in world coords.
	//rect A;
	//A.setStart({0,0}).setSize(size);
	//return toWorld(A);
	if(parent){
		return parent->toWorld(*this); //if has a parent, then we're in parent's coords.
	}else{
		return *this;	//else we're in world coords.
	}
}
rect rect::fromParent(rect A){	//A is in parent's coords, need in local coords.
	return A.setStart(fromParent(A.start)).setEnd(fromParent(A.end));
}
rect rect::fromParent(){	//need this rect in it's own coords?...
	rect A;
	A.setStart({0,0}).setSize(size);
	//return fromParent(A);
	return A;
}
rect rect::fromWorld(rect A){	//A is in world coords, need it in local.
	A.setStart(fromWorld(A.start)).setEnd(fromWorld(A.end));
}
rect rect::fromWorld(){	//uh
	//rect A;
	//A.setStart({0,0}).setSize(size);
	//return fromWorld(A);
	assert(!"dafuq does this even mean");
}

vector<rect> rect::getBorders(int borderSize){	//returns the four rects that correspond to the borders of this rect
//
//		a--ar-----------bl--b		a-b-bd-ad: rect 0 (upper border)
//		|	.			.	|		cu-du-d-c: rect 1 (lower border)
//		ad................. bd		a-ar-cr-c: rect 2 (left border)
//		|	.			.	|		b1-b-d-dl: rect 3 (right border)
//		|	.			.	|					rect 5 (inside the area but not touching any borders)
//		|	.			.	|
//		|	.			.	|
//		cu..................du
//		|	.			.	|
//		c--cr-----------dl--d
//
	rect upper,lower,left,right,center;
	upper.setStart(start).setEnd((vec2i){end.x, start.y+borderSize}).setParent(parent);
	lower.setStart((vec2i){start.x, end.y-borderSize}).setEnd(end).setParent(parent);
	left.setStart(start).setEnd((vec2i){start.x+borderSize, end.y}).setParent(parent);
	right.setStart((vec2i){end.x-borderSize, start.y}).setEnd(end).setParent(parent);
	if(borderSize > 0){
		center.setStart(start+(vec2i){borderSize,borderSize}).setEnd(end-(vec2i){borderSize,borderSize}).setParent(parent);
	}else{
		center = *this;
	}
	vector<rect> V;
	V.push_back(upper);
	V.push_back(lower);
	V.push_back(left);
	V.push_back(right);
	V.push_back(center);
	return V;
}
vec2i rect::toParent(vec2i A){
	return {A.x+start.x, A.y+start.y};
}
vec2i rect::toWorld(vec2i A){
	if(parent){
		return parent->toWorld(toParent(A));
	}else{
		return toParent(A);
	}
}
vec2i rect::fromParent(vec2i A){
	return {A.x-start.x, A.y-start.y};
}
vec2i rect::fromWorld(vec2i A){
	if(parent){
		return fromParent(parent->fromWorld(A));
	}else{
		return fromParent(A);
	}
}
bool operator == (rect A, rect B){
	return (A.getStart() == B.getStart()) && (A.getEnd() == B.getEnd());
}
vec2i rect::getStart(){return start;}
vec2i rect::getEnd(){return end;}
vec2i rect::getSize(){return size;}
vec2i rect::getTopLeft(){return (vec2i){start.x, start.y};}
vec2i rect::getTopRight(){return (vec2i){end.x, start.y};}
vec2i rect::getBottomLeft(){return (vec2i){start.x, end.y};}
vec2i rect::getBottomRight(){return (vec2i){end.x, end.y};}
vec2i rect::getCenter(){return (vec2i){(end.x+start.x)/2,(end.y+start.y)/2};}
int rect::getx(){return start.x;}
int rect::gety(){return start.y;}
int rect::getw(){return size.x;}
int rect::geth(){return size.y;}
int rect::getx2(){return end.x;}
int rect::gety2(){return end.y;}
rect *rect::getParent(){return parent;}
string rect::toString(){return string()+size.x+"x"+size.y+"/("+start.x+", "+start.y+")-("+end.x+", "+end.y+")";}
