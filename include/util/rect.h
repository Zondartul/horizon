#ifndef RECT_GUARD
#define RECT_GUARD
#include "util/vec2i.h"
#include "util/vec4i.h"
#include <string>
#include <vector>
using std::string;
using std::vector;
//a 2d rectangle with integer coordinates.
class rect{
	private:
	vec2i start;	//top-left vertex
	vec2i end;		//bottom-right vertex
	vec2i size;		//width and height.
	rect *parent;	//the rectangle that this rectangle's coordinates are relative to.
	public:
	rect();
	rect(vec2i start, vec2i end);
	rect &setStart(vec2i A);			//change dimensions (resize)
	rect &setEnd(vec2i A);
	rect &setSize(vec2i A);
	rect &setParent(rect *parent);
	rect &moveBy(vec2i A);				//move without resizing
	rect &moveStart(vec2i A);			
	rect &moveEnd(vec2i A);				
	rect &setx(int x);					//change dimensions (resize)
	rect &sety(int y);
	rect &setw(int w);
	rect &seth(int h);
	rect &setx2(int x2);
	rect &sety2(int y2);
	vec2i getStart();
	vec2i getEnd();
	vec2i getSize();
	vec2i getTopLeft();
	vec2i getTopRight();
	vec2i getBottomLeft();
	vec2i getBottomRight();
	vec2i getCenter();
	int getx();
	int gety();
	int getw();
	int geth();
	int getx2();
	int gety2();
	rect *getParent();
	bool contains(vec2i A); // is A inside of this?
	bool contains(vec4i A);
	bool contains(rect A);
	vec2i clamp(vec2i A);	// constrain A to be inside of this
	rect clamp(rect A);
	rect insert(rect A);	// move A inside of this
	rect toParent(rect A);	// convert to diffirent coordinate system
	rect toParent();
	rect toWorld(rect A);
	rect toWorld();
	rect fromParent(rect A);
	rect fromParent();
	rect fromWorld(rect A);
	rect fromWorld();
	vector<rect> getBorders(int borderSize);	//returns the four rects that correspond to the borders of this rect
	vec2i toParent(vec2i A);
	vec2i toWorld(vec2i A);
	vec2i fromParent(vec2i A);
	vec2i fromWorld(vec2i A);
	string toString();
};
bool operator == (rect A, rect B);
#endif